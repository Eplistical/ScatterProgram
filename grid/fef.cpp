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

static void _electronic_fef(const double hbar,
							const double bandwidth,
							const int dim, 
                            const double kT,
                            const double de_min,
                            const double de_max,
                            const double h,
                            const double * const nabla_h, 
                            const double gamma, 
                            const double * const nabla_gamma, 
                            double * const force, 
                            double * const efric, 
                            double * const fBCME)
{
    /* core function to calculate force, efric, fBCME 
     * the calculated forces do not include nuclear force
     * for given h, h', gamma, gamma'
     * written in C for speed */
    const int dim2 = dim * dim;
    memset(force, 0, sizeof(double) * dim);
    memset(efric, 0, sizeof(double) * dim2);
    memset(fBCME, 0, sizeof(double) * dim);
    // numerical integration from -bandwidth to bandwidth
    // use kT to determine de
    const double de = max(min(0.05 * kT, de_max), de_min);
    const size_t INT_N = static_cast<size_t>(bandwidth * 2 / de) + 1;
    double kernel[dim];
    double e, f, dfde, A, tmp;
    double n = 0.0;
    for(int k = 0; k < INT_N; ++k){
        // f, f', A
        e = -bandwidth + k * de;
        f = Fermi(e / kT);
        dfde = -f * (1 - f) / kT;
        A = Lorentian(e - h, gamma / 2);
        // kernel = h' + (e - h) * gamma' / gamma
        for(int d = 0; d < dim; ++d){
            kernel[d] = nabla_h[d] + nabla_gamma[d] * (e - h) / gamma;
        }
        // force
        tmp = A * f * de;
        for(int d = 0; d < dim; ++d){
            force[d] -= tmp * kernel[d];
        }
        n += tmp;
        tmp = M_PI * hbar * dfde * A * A * de;
        for(int d = 0;d < dim; ++d){
            for(int d2 = 0; d2 <= d; ++d2){
                efric[d2 + dim * d] -= tmp * kernel[d] * kernel[d2];
            }
        }
    }
    // symmetrize efric
    for(int d = 0; d < dim; ++d){
        for(int d2 = d + 1; d2 < dim; ++d2){
            efric[d2 + dim * d] = efric[d + dim * d2];
        }
    }
    // calc fBCME
    tmp = n - Fermi(h / kT);
    for(int d = 0; d < dim; ++d){
        fBCME[d] = -nabla_h[d] * tmp;
    }
}

} // extern "C"


//  calc fef between S0 & S1, for a single point r (labeled by index)
// 	S0 has fewer el
void scatter::grid_t::calc_fef(	size_t S0, 
								size_t S1, 
								size_t index)
{
	using namespace scatter::rem;
	using namespace scatter::grid;
	using scatter::surfaces_obj;
	const std::vector<double> r = index_to_r(index);
	const size_t Ntot = get_Ntot();
	double * const force = &_fef[index * dim];
	double * const efric = &_fef[_forcelen + index * dim2];
	double * const fBCME = &_fef[_forcelen + _efriclen + index * dim];
    // h, h'
    const double h = surfaces_obj.fU(S1, r) - surfaces_obj.fU(S0, r);
    const std::vector<double> nabla_h = surfaces_obj.fF(S0, r) - surfaces_obj.fF(S1, r);
    if(surfaces_obj.small_gamma(r, Gamma0)) {
        /* if smallgamma, no friction or broaden */
        const double tmp = Fermi(h / kT);
        for(int d = 0; d < dim; ++d){
            force[d] -= nabla_h[d] * tmp;
        }
        memset(efric, 0, sizeof(double) * dim2);
        memset(fBCME, 0, sizeof(double) * dim);
    }
    else {
        /* non-small gamma
         * invoke numerical integral */
        const double gamma = surfaces_obj.fGamma(r, Gamma0);
        const std::vector<double> nabla_gamma = surfaces_obj.fGammader(r, Gamma0);
        // integrate calc electronic force(r), efric(r), fBCME(r)
        _electronic_fef(hbar, bandwidth, 
						dim, kT,
						derange[0], derange[1],
                        h, &nabla_h[0], 
                        gamma, &nabla_gamma[0],
                        force, efric, fBCME);
    }
    // add nuclear force term
    for(size_t d = 0; d < dim; ++d) {
        force[d] += surfaces_obj.fF(S0, d, r[d]);
    }
}

