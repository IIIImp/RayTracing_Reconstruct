#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

class sphere : public hittable {
private:
	point3 center;
	double radius;
	shared_ptr<material> mat;
public:
	sphere(const point3 center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 round_center2ray_origin = center - r.get_Origin();
		auto a = dot(r.get_Direction(), r.get_Direction());
		auto h = dot(r.get_Direction(), round_center2ray_origin);
		auto c = dot(round_center2ray_origin, round_center2ray_origin) - radius * radius;
		auto discriminant = h * h - a * c;
		if (discriminant < 0) return false;

		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}
		rec.hit_ray_t = root;
		rec.hit_point = r.at(root);
		vec3 outward_normal = (rec.hit_point - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}
};

#endif