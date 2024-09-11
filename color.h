//
// Created by Marical on 2023-12-22.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;

/// Gamma校正
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component); //Gamma = 2，一般为2.2，这里做了近似
    return 0;
}


void write_color(std::ostream &out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();


    // Gamma校正
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //将值限制在0~255间
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));


    //放到输出流中
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';



}

#endif
