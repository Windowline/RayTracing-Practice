#ifndef HITTABLE_H
#define HITTABLE_H
#include "interval.h"
#include "ray.h"
#include "rtweekend.h"

#include <memory>

class material;

class hit_record {
public:
    vec3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
//    virtual ~hittable() = default;

//    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif