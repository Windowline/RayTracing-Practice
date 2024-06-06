#include <iostream>
#include "src/rtweekend.h"
#include "src/color.h"
#include "src/camera.h"
#include "src/hittable.h"
#include "src/hittable_list.h"
#include "src/sphere.h"
#include "src/material.h"
#include "src/bvh.h"
#include "src/texture.h"
#include "src/quad.h"

inline shared_ptr<hittable_list> box(const vec3& a, const vec3& b, shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = vec3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = vec3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(vec3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(vec3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}

void cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(vec3(.65, .05, .05));
    auto white = make_shared<lambertian>(vec3(.73, .73, .73));
    auto green = make_shared<lambertian>(vec3(.12, .45, .15));
    auto light = make_shared<diffuse_light>(vec3(15, 15, 15));

    world.add(make_shared<quad>(vec3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(vec3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(vec3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(vec3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

//    world.add(box(vec3(130, 0, 65), vec3(295, 165, 230), white));
//    world.add(box(vec3(265, 0, 295), vec3(430, 330, 460), white));
    shared_ptr<hittable> box1 = box(vec3(0,0,0), vec3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(vec3(0,0,0), vec3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 5;
//    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = vec3(278, 278, -800);
    cam.lookat   = vec3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

//    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(vec3(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(vec3(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(vec3(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(vec3(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(vec3(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(vec3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(vec3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(vec3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(vec3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = vec3(0,0,9);
    cam.lookat   = vec3(0,0,0);
    cam.vup      = vec3(0,1,0);

//    cam.defocus_angle = 0;

    cam.render(world);
}

void bouncing_spheres() {
    hittable_list world;

//    auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));
    auto checker = make_shared<checker_texture>(0.32, vec3(.2, .3, .1), vec3(.9, .9, .9));
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(checker)));


    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = vec3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;

//    cam.image_width       = 1200;
//    cam.samples_per_pixel = 500;
//    cam.max_depth         = 50;

    cam.image_width       = 200;
    cam.samples_per_pixel = 1;
    cam.max_depth         = 4;

    cam.vfov     = 20;
    cam.lookfrom = vec3(13,2,3);
    cam.lookat   = vec3(0,0,0);
    cam.vup      = vec3(0,1,0);
//    cam.defocus_angle = 0.6;
//    cam.focus_dist    = 10.0;
    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<image_texture>("res/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(vec3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = vec3(0,0,12);
    cam.lookat   = vec3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.render(hittable_list(globe));
}


int main() {
//    bouncing_spheres();
//    earth();
//    quads();
    cornell_box();
}