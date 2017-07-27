Program for high dimensional scattering problem, comparing behaviours of 

* Classical master equation (CME)
* Broadened classical master equation (BCME) 
* Electronic friction (EF).

Require:
* C++ compiler supporting C++11 feature.
* MPI library, e.g. [OpenMPI](https://www.open-mpi.org/)
* [Boost C++ library](http://www.boost.org/) with version >= 1.44.0


To install:
```
 git clone https://github.com/Eplistical/ScatterProgram.git
 
 cd ScatterProgram

 mkdir build

 cd build

 cmake -DCMAKE_INSTALL_PREFIX=<install_prefix> ..

 make

 make install
```
