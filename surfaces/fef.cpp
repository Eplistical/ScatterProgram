#include "vector.hpp"
#include "rem.hpp"
#include "fermi.hpp"
#include "lorentian.hpp"
#include "surfaces_collection.hpp"
#include "surfaces_t.hpp"
#include "surfaces_var.hpp"

#ifdef __cpluscplus
extern "C"{
#endif
static void _electronic_fef(const double hbar,
							const double BANDWIDTH,
							const int dim, 
                            const double kT,
                            const double h,     
                            const double * const nabla_h, 
                            const double gamma, 
                            const double * const nabla_gamma, 
                            double * const force, 
                            double * const efric, 
                            double * const fBCME){
    /* core function to calculate force, efric, fBCME 
     * the calculated forces do not include nuclear force
     * for given h, h', gamma, gamma'
     * written in C for speed */
    const int dim2 = dim * dim;
    memset(force, 0, sizeof(double) * dim);
    memset(efric, 0, sizeof(double) * dim2);
    memset(fBCME, 0, sizeof(double) * dim);
    // numerical integration from -BANDWIDTH to BANDWIDTH
    // use kT to determine de
    const double de = 0.05 * kT;
    const size_t INT_N = static_cast<size_t>(BANDWIDTH * 2 / de) + 1;
    double kernel[dim];
    double e, f, dfde, A, tmp;
    double n = 0.0;
    for(int k = 0;k < INT_N;k++){
        // f, f', A
        e = -BANDWIDTH + k * de;
        f = Fermi(e / kT);
        dfde = -f * (1 - f) / kT;
        A = Lorentian(e - h, gamma / 2);
        // kernel = h' + (e - h) * gamma' / gamma
        for(int d = 0;d < dim;d++){
            kernel[d] = nabla_h[d] + nabla_gamma[d] * (e - h) / gamma;
        }
        // force
        tmp = A * f * de;
        for(int d = 0; d < dim; d++){
            force[d] -= tmp * kernel[d];
        }
        n += tmp;
        tmp = M_PI * hbar * dfde * A * A * de;
        for(int d = 0;d < dim;d++){
            for(int d2 = 0; d2 <= d; d2++){
                efric[d2 + dim * d] -= tmp * kernel[d] * kernel[d2];
            }
        }
    }
    // symmetrize efric
    for(int d = 0;d < dim;d++){
        for(int d2 = d + 1; d2 < dim; d2++){
            efric[d2 + dim * d] = efric[d + dim * d2];
        }
    }
    // calc fBCME
    tmp = n - Fermi(h / kT);
    for(int d = 0;d < dim;d++){
        fBCME[d] = -nabla_h[d] * tmp;
    }
}
#ifdef __cpluscplus
}
#endif

using namespace scatter;

// calc fef
void surfaces_t::fef(const std::vector<double>& r, double *force, double *efric, double *fBCME){
	using namespace scatter::rem;
	using namespace scatter::surfaces;
    // h, h'
    const double h = fU(1, r) - fU(0, r);
    const std::vector<double> nabla_h = fF(0, r) - fF(1, r);
    if(small_gamma(r, Gamma0)){
        /* if smallgamma, A->delta function
         * force = -h'*f
         * no friction or broaden */
        const double tmp = Fermi(h / kT);
        for(int d = 0;d < dim;d++){
            force[d] = force[d] - nabla_h[d] * tmp;
        }
        memset(efric, 0, sizeof(double) * dim2);
        memset(fBCME, 0, sizeof(double) * dim);
    }
    else{
        /* non-small gamma
         * invoke numerical integral */
        const double gamma = fGamma(r, Gamma0);
        const std::vector<double> nabla_gamma = fGammader(r, Gamma0);
        // integrate calc electronic force(r), efric(r), fBCME(r)
        _electronic_fef(hbar, bandwidth, 
						dim, kT,
                        h, &nabla_h[0], 
                        gamma, &nabla_gamma[0],
                        force, efric, fBCME);
    }
    // add nuclear force term
    for(int d = 0;d < dim;d++){
        force[d] = force[d] + fF(0, r)[d];
    }
}
