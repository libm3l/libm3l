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


[![Analytics](https://ga-beacon.appspot.com/UA-47981720-2/libm3l/libm3l)](https://github.com/igrigorik/ga-beacon)
