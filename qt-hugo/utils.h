#ifndef UTILS_H
#define UTILS_H

#endif // UTILS_H

#include "Vertex.h"
#include <cmath>

const double earthRadius = 6371000;

void latLonToXY(Vertex *vertex,double longitude , double latitude){


    double latRad  = latitude  * M_PI / 180.0;
    double longRad = longitude * M_PI / 180.0;

    vertex->setX(earthRadius*longRad);
    vertex->setY(earthRadius*log(tan(M_PI/4 + latRad/2)));
}
