
%module ugdk_math

%include <module/export.swig>

%{

#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>

%}

%ignore operator+(const Vector2D &left, const Vector2D &right);
%ignore operator-(const Vector2D &left);
%ignore operator-(const Vector2D &left, const Vector2D &right);
%ignore operator*(const float  &scalar, const Vector2D &right);
%ignore operator*(const Vector2D &left, const float   &scalar);
%ignore operator/(const Vector2D &left, const float   &scalar);
%ignore operator*(const Vector2D &left, const Vector2D &right);

%include <ugdk/math/vector2D.h>
%include <ugdk/math/frame.h>

namespace ugdk {
    export_class(Vector2D)
    export_class(Frame)
}


