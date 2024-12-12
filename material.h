#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return false;
	}
};

class lambertian : public material {
private:
	color albedo;
public:
	lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
		auto scatter_direction = rec.hit_normal + random_unit_vector();

		if (scatter_direction.near_zero()) {
			scatter_direction = rec.hit_normal;
		}

		scattered = ray(rec.hit_point, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
private:
	color albedo;
	double fuzz;
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.get_Direction(), rec.hit_normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = ray(rec.hit_point, reflected);
		attenuation = albedo;
		return (dot(scattered.get_Direction(), rec.hit_normal) > 0);
	}
};

class dielectric : public material {
private:
	double refraction_index;

	static double reflectance(double cosine, double refraction_index) {
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
public:
	dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.get_Direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.hit_normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
			direction = reflect(unit_direction, rec.hit_normal);
		}
		else {
			direction = refract(unit_direction, rec.hit_normal, ri);
		}

		scattered = ray(rec.hit_point, direction);
		return true;
	}
};

#endif // !MATERIAL_H

