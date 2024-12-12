#ifndef PARAM_H
#define PARAM_H

////////////// Camera Parameters //////////////

// Image Set
const float Custom_aspect_ratio = 16.0 / 9.0;
const int   Custom_image_width  = 400;

// Camera's Eye
const float Custom_vfov          = 20;
const float Custom_defocus_angle = 0.6;
const float Custom_focus_dist    = 10.0;

// Camera Position
const vec3 Custom_lookfrom = point3(13, 2, 3);
const vec3 Custom_lookat   = point3(0, 0, 0);
const vec3 Custom_vup      = point3(0, 1, 0);

////////////// World Parameters //////////////

// First weekend World
const float ground_radius = 1000;
const int   object_nums   = 11;

////////////// Render Parameters //////////////

// MSAA Parameter
const int Custom_samples_per_pixel = 5;

// Recursion depth
const int Custom_max_depth = 10;

#endif // !PARAM_H

