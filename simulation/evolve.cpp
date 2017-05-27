#include "rem.hpp"
#include "vector.hpp"
#include "simulation_var.hpp"
#include "evolve.hpp"

using namespace scatter::simulation;
using scatter::particle_t;

static std::vector<double> RK4_f(const particle_t& ptcl, int mode){
    return ptcl.p / scatter::simulation::mass;
}

// API
void scatter::simulation::velocity_verlet(particle_t& ptcl, int mode, forcefunc_t forcefunc){
    std::vector<double>&& force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
    ptcl.r = ptcl.r + dt * ptcl.p / mass;
    force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
}

void scatter::simulation::runge_kutta_4th(particle_t& ptcl, int mode, forcefunc_t RK4_g){
	const forcefunc_t f = RK4_f;
	const forcefunc_t g = RK4_g;
    // RK4
    particle_t tmp_ptcl = ptcl;

    const std::vector<double>&& k1 = f(tmp_ptcl, mode);
    const std::vector<double>&& l1 = g(tmp_ptcl, mode);

    tmp_ptcl.r = tmp_ptcl.r + 0.5 * dt * k1;
    tmp_ptcl.p = tmp_ptcl.p + 0.5 * dt * l1;
    const std::vector<double>&& k2 = f(tmp_ptcl, mode);
    const std::vector<double>&& l2 = g(tmp_ptcl, mode);

    tmp_ptcl.r = tmp_ptcl.r + 0.5 * dt * (k2 - k1);
    tmp_ptcl.p = tmp_ptcl.p + 0.5 * dt * (l2 - l1);
    const std::vector<double>&& k3 = f(tmp_ptcl, mode);
    const std::vector<double>&& l3 = g(tmp_ptcl, mode);

    tmp_ptcl.r = tmp_ptcl.r + dt * k3 - 0.5 * dt * k2;
    tmp_ptcl.p = tmp_ptcl.p + dt * l3 - 0.5 * dt * l2;
    const std::vector<double>&& k4 = f(tmp_ptcl, mode);
    const std::vector<double>&& l4 = g(tmp_ptcl, mode);

    ptcl.r = ptcl.r + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
    ptcl.p = ptcl.p + dt / 6 * (l1 + 2 * l2 + 2 * l3 + l4);
}
