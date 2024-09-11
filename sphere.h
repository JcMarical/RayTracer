//
// Created by lincolnjcli on 2024/7/21.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "ray.h"
#include "hittable.h"

class sphere : public hittable{
public:
        //constructor
        sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(fmax(0,radius)),mat(mat) {
            //TODO:
        }


        //functions
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            //计算根
                vec3 oc = center - r.origin();
                auto a = r.direction().length_squared();  //方向的长度方
                auto h = dot(r.direction(), oc); //方向和原点的距离
                auto c = oc.length_squared() - radius * radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0)
                return false;

            //计算交点
            auto sqrtd = sqrt(discriminant);
            // 如果这个t值不在有效范围内，尝试计算第二个交点的t值
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                // 如果第二个t值也不在有效范围内，说明射线与球体不相交
                if (!ray_t.surrounds(root))
                    return false;
            }

            //确定正反面
                rec.t = root;
                rec.p = r.at(rec.t);
                vec3 outward_normal = (rec.p - center) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat = mat;

            return true;
        }


private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif //RAYTRACER_SPHERE_H
