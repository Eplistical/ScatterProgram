#include "types.hpp"
#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "vector.hpp"
#include "rem.hpp"
#include "fermi.hpp"
#include "lorentian.hpp"
#include "surfaces.hpp"
#include "grid_var.hpp"
#include "grid_t.hpp"
#include "run.hpp"

extern "C"
{

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

static void _electronic_fef(const DOUBLE_T hbar,
							const DOUBLE_T bandwidth,
							const INT_T dim, 
                            const DOUBLE_T kT,
                            const DOUBLE_T de_min,
                            const DOUBLE_T de_max,
                            const DOUBLE_T h,
                            const DOUBLE_T * const nabla_h, 
                            const DOUBLE_T gamma, 
                            const DOUBLE_T * const nabla_gamma, 
                            DOUBLE_T * const force, 
                            DOUBLE_T * const efric, 
                            DOUBLE_T * const fBCME)
{
    /* core function to calculate force, efric, fBCME 
     * the calculated forces do not include nuclear force
     * for given h, h', gamma, gamma'
     * written in C for speed */
    const INT_T dim2 = dim * dim;
    memset(force, 0, sizeof(DOUBLE_T) * dim);
    memset(efric, 0, sizeof(DOUBLE_T) * dim2);
    memset(fBCME, 0, sizeof(DOUBLE_T) * dim);
    // numerical integration from -bandwidth to bandwidth
    // use kT to determine de
    const DOUBLE_T de = max(min(0.05 * kT, de_max), de_min);
    const UINT_T INT_N = static_cast<UINT_T>(bandwidth * 2 / de) + 1;
    DOUBLE_T kernel[dim];
    DOUBLE_T e, f, dfde, A, tmp;
    DOUBLE_T n = 0.0;
    for(INT_T k = 0; k < INT_N; ++k){
        // f, f', A
        e = -bandwidth + k * de;
        f = Fermi(e / kT);
        dfde = -f * (1 - f) / kT;
        A = Lorentian(e - h, gamma / 2);
        // kernel = h' + (e - h) * gamma' / gamma
        for(INT_T d = 0; d < dim; ++d){
            kernel[d] = nabla_h[d] + nabla_gamma[d] * (e - h) / gamma;
        }
        // force
        tmp = A * f * de;
        for(INT_T d = 0; d < dim; ++d){
            force[d] -= tmp * kernel[d];
        }
        n += tmp;
        tmp = M_PI * hbar * dfde * A * A * de;
        for(INT_T d = 0;d < dim; ++d){
            for(INT_T d2 = 0; d2 <= d; ++d2){
                efric[d2 + dim * d] -= tmp * kernel[d] * kernel[d2];
            }
        }
    }
    // symmetrize efric
    for(INT_T d = 0; d < dim; ++d){
        for(INT_T d2 = d + 1; d2 < dim; ++d2){
            efric[d2 + dim * d] = efric[d + dim * d2];
        }
    }
    // calc fBCME
    tmp = n - Fermi(h / kT);
    for(INT_T d = 0; d < dim; ++d){
        fBCME[d] = -nabla_h[d] * tmp;
    }
}

} // extern "C"


//  calc fef between S0 & S1, for a single poINT_T r (labeled by index)
// 	S0 has fewer el
void scatter::grid_t::calc_fef(	UINT_T S0, 
								UINT_T S1, 
								UINT_T index)
{
	using namespace scatter::rem;
	using namespace scatter::grid;
	using scatter::surfaces_obj;
	const std::vector<DOUBLE_T> r = index_to_r(index);
	const UINT_T Ntot = get_Ntot();
	DOUBLE_T * const force = &_fef[index * dim];
	DOUBLE_T * const efric = &_fef[_forcelen + index * dim2];
	DOUBLE_T * const fBCME = &_fef[_forcelen + _efriclen + index * dim];
    // h, h'
    const DOUBLE_T h = surfaces_obj.fU(S1, r) - surfaces_obj.fU(S0, r);
    const std::vector<DOUBLE_T> nabla_h = surfaces_obj.fF(S0, r) - surfaces_obj.fF(S1, r);
    if(surfaces_obj.small_gamma(r, Gamma0)) {
        /* if smallgamma, no friction or broaden */
        const DOUBLE_T tmp = Fermi(h / kT);
        for(INT_T d = 0; d < dim; ++d){
            force[d] -= nabla_h[d] * tmp;
        }
        memset(efric, 0, sizeof(DOUBLE_T) * dim2);
        memset(fBCME, 0, sizeof(DOUBLE_T) * dim);
    }
    else {
        /* non-small gamma
         * invoke numerical integral */
        const DOUBLE_T gamma = surfaces_obj.fGamma(r, Gamma0);
        const std::vector<DOUBLE_T> nabla_gamma = surfaces_obj.fGammader(r, Gamma0);
        // integrate calc electronic force(r), efric(r), fBCME(r)
        _electronic_fef(hbar, bandwidth, 
						dim, kT,
						derange[0], derange[1],
                        h, &nabla_h[0], 
                        gamma, &nabla_gamma[0],
                        force, efric, fBCME);
    }
    // add nuclear force term
    for(UINT_T d = 0; d < dim; ++d) {
        force[d] += surfaces_obj.fF(S0, d, r[d]);
    }
}

