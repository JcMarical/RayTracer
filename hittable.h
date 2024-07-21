//
// Created by lincolnjcli on 2024/7/21.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "ray.h"
#include "interval.h"

class hit_record{

    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal){
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable{
    public:
        virtual ~hittable() = default; //析构

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};






#endif //RAYTRACER_HITTABLE_H
