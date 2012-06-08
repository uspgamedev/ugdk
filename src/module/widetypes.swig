/*
This swig helper module defines the necessary conversion stuff for wide types.
Most notably, wstrings.
*/

#ifdef SWIGLUA

#elif SWIGPYTHON
%include "std_wstring.i"
#endif
