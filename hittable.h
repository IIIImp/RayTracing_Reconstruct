#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;

class hit_record {
public:
	point3 hit_point;
	vec3 hit_normal;
	shared_ptr<material> mat;
	double hit_ray_t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.get_Direction(), outward_normal) < 0;
		hit_normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
