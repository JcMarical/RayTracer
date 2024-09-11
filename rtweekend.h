//
// Created by lincolnjcli on 2024/7/21.
//

#ifndef RAYTRACER_RTWEEKEND_H
#define RAYTRACER_RTWEEKEND_H


#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


inline double random_double() {
    // 随机数[0,1).左闭右开
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // 随机数[min,max).
    return min + (max-min)*random_double();
}



// Common Headers

#include "ray.h"
#include "vec3.h"
#include "color.h"



#endif //RAYTRACER_RTWEEKEND_H
