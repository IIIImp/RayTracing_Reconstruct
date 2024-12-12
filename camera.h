#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "color.h"
#include "ray.h"
#include "param.h"

#include <vector>
#include <iostream>
#include <omp.h>

class camera {
public:	
	camera() { initialize(); }

	int getImage_height() const { return image_height; }
	int getSamples_per_pixel() const { return samples_per_pixel; }
	int getMax_depth() const { return max_depth; }
	float getPixel_samples_scale() const { return pixel_samples_scale; }
	float getImage_width() const { return image_width; }
private:
	double aspect_ratio = Custom_aspect_ratio;
	int    image_width = Custom_image_width;
	int    samples_per_pixel = Custom_samples_per_pixel;
	int    max_depth = Custom_max_depth;
	double vfov = Custom_vfov;
	point3 lookfrom = Custom_lookfrom;
	point3 lookat = Custom_lookat;
	vec3   vup = Custom_vup;
	double defocus_angle = Custom_defocus_angle;
	double focus_dist = Custom_focus_dist;

	int    image_height;        // Camera image height
	double pixel_samples_scale;
	point3 camera_center;    // Camera Center
	point3 pixel00_center;   // Location of pixel (0, 0)
	vec3   pixel_delta_u;      // Offset to pixel moving right
	vec3   pixel_delta_v;      // Offset to pixel moving down
	vec3   u, v, w;
	vec3   defocus_disk_u;
	vec3   defocud_disk_v;

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = image_height < 1 ? 1 : image_height;
		
		pixel_samples_scale = 1.0 / samples_per_pixel;

		camera_center = lookfrom;

		auto theta = degrees_to_radians(vfov);
		auto h = std::tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// Calculate u, v ,w
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// Get v_u and v_v
		auto viewport_u = viewport_width * u;
		auto viewport_v = viewport_height * -v;

		// Get delta u, v
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Put the viewport properly
		auto viewport_upper_left = camera_center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
		
		// Get the center of pixel
		pixel00_center = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
		
		auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocud_disk_v = v * defocus_radius;
	}
public:	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel00_center + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

		auto ray_origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}
	point3 defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocud_disk_v);
	}
public:	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0) return color(0, 0, 0);
		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			return color(0, 0, 0);
		}
		vec3 ray_unit_direction = unit_vector(r.get_Direction());
		auto a = 0.5 * (ray_unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif // !CAMERA_H
