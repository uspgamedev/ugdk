
%define enable_disown(argexp)
%apply SWIGTYPE* DISOWN {argexp};
%enddef

%define disable_disown(argexp)
%clear argexp;
%enddef
