[![Build Status](https://travis-ci.org/dinocore1/Baseline.svg?branch=master)](https://travis-ci.org/dinocore1/Baseline)

# Baseline #

A collection of C++ utility classes. Baseline's intention is to serve as an
alternative to the C++ Standard Template Library (STL). The library utilizes
copy-on-write as a resource-management technique. All of the container classes
(Vector, Strings, Maps, etc) make use of SharedBuffer class. This allows for 
efficent copy/assignment operators and low memory bloat. 


### Container Classes ###

 * Vector - dynamic array-like class
 * SortedVector - like a Vector, but items are sorted according to their natural order (via < operator).
 * CircleBuffer

### Memory/Garbage Collection ###

  * UniquePointer
  * Strong/Weak Pointer

 ### Threading ###

  * Thread - uses either pthreads or Win32 Threads based on target OS
  * Mutex/Autolock
  * Condition
  * Atomic

### Other ###

 * String8/16 - support unicode

### Math ###
 
 * StorelessStats - compute mean, variance, std. deviation, etc.