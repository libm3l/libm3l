libm3l - A data protocol for transfer of basic data stored in a linked list over TCP/IP
=======================================================================================

libm3l is a utility which stores the basic data type (float, double, char, int ...) in 
a linked list and enables its transfer over the TCP/IP to a waiting process.
The library enables operations with a linked list such as make list, add list, copy list, remove list,
link list etc.


Programming Language
--------------------

Programming language is ANSI-C

Installation
---------------

Got to Source/data_util and type make. The dynamic library and header file libm3l.h are going to be located in this 
directory

Linking
-----------

To link the shared library use 

-L/PATHTOLIBM3L/ -lm3l -Wl,-rpath=/PATHTOLIBM3L/ 

where PATHTOLIBM3L is a path to location of libm3l.so file 

Portability
-----------

libm3l is primarily for Linux. After some modifications it can be used
on Windows systems, the necessary modifications are not part of this distribution.

References
-----------

Adam Jirasek and Arthur W. Rizzi: "libm3l and lsipdx - Utilities for Inter-Process Data Transfer and Synchronization", 52nd Aerospace Sciences Meeting, AIAA SciTech Forum, (AIAA 2014-1045) https://doi.org/10.2514/6.2014-1045, https://www.researchgate.net/publication/269248697_libm3l_and_lsipdx_-_Utilities_for_Inter-Process_Data_Transfer_and_Synchronization

 Adam Jirasek, Olivier Amoignon, Peter Eliasson, Mats Dalenbring and Urban Falk: "Coupling of the Edge CFD Solver with External Solvers", 53rd AIAA Aerospace Sciences Meeting, AIAA SciTech Forum, (AIAA 2015-0769) https://doi.org/10.2514/6.2015-0769, https://www.researchgate.net/publication/273462889_Coupling_of_the_Edge_CFD_Solver_with_External_Solvers

Karlsson A. and Ringertz U.: Experimental and Analytical Investigation of Aircraft-Like Wind Tunnel Model with External Stores, XVII International Forum on Aeroleasticity and Structural Dynamics, 25-28 June, 2017, Como, Italy



[![Analytics](https://ga-beacon.appspot.com/UA-47981720-2/libm3l/libm3l)](https://github.com/igrigorik/ga-beacon)
