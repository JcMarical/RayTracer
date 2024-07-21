//
// Created by lincolnjcli on 2024/7/21.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H


#include "hittable.h"


#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable{
    public:
        std::vector<shared_ptr<hittable>> objects;
        //ctor
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }


        //functions
        //----------------------------
        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        //override Hit
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            //遍历所有物体，找到光线最后碰撞的那个物体距离
            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }


};



#endif //RAYTRACER_HITTABLE_LIST_H
