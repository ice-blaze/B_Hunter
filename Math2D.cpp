#include "Math2D.h"
#include <cmath>

Math2D::Math2D()
{
    //ctor
}

Math2D::~Math2D()
{
    //dtor
}

void Math2D::extendSegment(double &_xD,double &_yD, int _size){
    double normeD = sqrt(_xD*_xD+_yD*_yD);
    _xD /= normeD;
    _yD /= normeD;
    _xD *= _size;
    _yD *= _size;
}
