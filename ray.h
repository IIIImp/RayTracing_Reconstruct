#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
private:
	point3 ray_Origin;
	vec3 ray_Direction;
public:
	ray() {}

	ray(const point3& ray_Origin_, const vec3& ray_Direction_) : ray_Origin(ray_Origin_), ray_Direction(ray_Direction_) {}

	const point3& get_Origin() const { return ray_Origin; }
	const vec3& get_Direction() const { return ray_Direction; }

	point3 at(double t) const {
		return ray_Origin + t * ray_Direction;
	}
};


#endif