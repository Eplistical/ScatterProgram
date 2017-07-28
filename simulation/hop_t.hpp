#ifndef _SCATTER_HOP_T_HPP
#define _SCATTER_HOP_T_HPP
// module for single particle class
#include "scatter_basic.hpp"
#include "vector.hpp"

namespace scatter 
{
    namespace simulation
    {
        struct hop_t
        {
            UINT_T id;
            UINT_T from;
            UINT_T to;
            std::vector<DOUBLE_T> r;
            std::vector<DOUBLE_T> p;
            DOUBLE_T gamma;
            DOUBLE_T energy_gap;

            explicit hop_t(UINT_T ID, UINT_T From, UINT_T To, DOUBLE_T EnergyGap, DOUBLE_T Gamma, const std::vector<DOUBLE_T>& R, const std::vector<DOUBLE_T>& P)
                : id(ID), from(From), to(To), energy_gap(EnergyGap), gamma(Gamma), r(R), p(P) {}
            hop_t(const hop_t& other) = default;
            ~hop_t() = default;
            hop_t& operator=(const hop_t& other) = default;
        };
    };
};

#endif
