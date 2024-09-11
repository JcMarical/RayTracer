//
// Created by lincolnjcli on 2024/7/22.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

#include "hittable.h"
#include "material.h"

class camera {
public:
    /* 在这里设置相机的参数 */
    double aspect_ratio = 1.0;
    int image_width = 100;
    int    samples_per_pixel = 10;    //采样数
    int    max_depth         = 10;    //递归最大深度



    double vfov     = 90;              // 直角视野
    point3 lookfrom = point3(0,0,0);   // Point camera is looking from
    point3 lookat   = point3(0,0,-1);  // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    /* 散焦设置 */
    double defocus_angle = 0;  // 散焦角度
    double focus_dist = 10;    // 相机焦距





    void render(const hittable &world) {
        initialize();

        //Render

        std::cout << "P3\n" << image_width << ' ' << image_height <<"\n255\n";

        for(int j = 0;j < image_height; j++){
            std::clog << "\rScanLines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                // 采样，计算每个周围随机像素的颜色
                for (int sample = 0;sample <samples_per_pixel;sample++)
                {
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout,pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    /* 相机的私有变量 */
    int    image_height;
    double pixel_samples_scale; // 采样缩放因子
    point3 center;         // Camera center
    point3 pixel00_loc;    // 初始位置
    vec3   pixel_delta_u;  // 像素增量
    vec3   pixel_delta_v;  // 像素增量
    vec3   u, v, w;        // 相机帧基础向量
    vec3   defocus_disk_u; // 散焦光点u基础向量
    vec3   defocus_disk_v; // 散焦光点v基础向量

    void initialize() {
        image_height = static_cast<int>(image_width/aspect_ratio);
        image_height = (image_height<1)? 1 : image_height;

        center= point3(0,0,0);

        //采样因子 = 1/采样数
        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        //决定视口的维度
        //auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);  // 90度变弧度制
        auto h = tan(theta/2);//tan45==1，一半的高与焦距比
        auto viewport_height = 2 * h * focus_dist; //
        //aspect_radio is ideal...
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        //计算相机坐标视口uv边缘向量
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);




        //计算视口uv边缘向量
        //uv
        //auto viewport_u = vec3(viewport_width,0,0);
        //auto viewport_v = vec3(0,-viewport_height,0);
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;


        //计算像素增量
        pixel_delta_u = viewport_u/image_width;
        pixel_delta_v = viewport_v/image_height;

        //计算左上像素
        //auto viewport_upper_left = center- vec3(0,0,focal_length)- viewport_u/2- viewport_v/2;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;

        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


        //计算散焦基础向量
        auto defocus_disk_radius = focus_dist * tan(degrees_to_radians(defocus_angle));
        defocus_disk_u = defocus_disk_radius * u;
        defocus_disk_v = defocus_disk_radius * v;
    }

    /// 从相机位置发射一条有随机偏移的射线
    ray get_ray(int i, int j) const {

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                + ((j + offset.y()) * pixel_delta_v);

        /// 通过散焦角度设置，来决定画面是否进行散焦模糊处理
        auto ray_origin = (defocus_angle<=0)? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);


    }

    /// 返回一个随机向量，范围[-0.5,0.5]
    /// \return vec3
    vec3 sample_square() const {

        return vec3(random_double()-0.5,random_double()-0.5,0);
    }


    point3 defocus_disk_sample() const {
        //返回一个相机位置的散焦盘随机点
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r,int depth, const hittable& world) const {
        /*
        auto t = hit_sphere(point3(0,0,-1), 0.5, r);
        if (t > 0.0) {
            vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
            return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
        }
        */
        if(depth <= 0)
            return color(0,0,0);

        //计算ray与球体的交点
        hit_record rec;
        if(world.hit(r, interval(0.0001, infinity), rec)){
            //return 0.5*color(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
            //vec3 direction = random_on_hemisphere(rec.normal);

            //vec3 direction = rec.normal + random_unit_vector(); //Lambertian reflection 计算
            //递归取色（再次发出光线）
            //return 0.1 * ray_color(ray(rec.p, direction),depth-1, world);

            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        //天空背景色
        vec3 unit_direction = unit_vector(r.direction());
        //
        auto a = 0.5*(unit_direction.y() + 1.0);//[0,1]高度计算颜色
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

};
#endif //RAYTRACER_CAMERA_H
