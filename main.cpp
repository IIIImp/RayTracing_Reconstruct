#include "rtweekend.h"

#include <fstream>

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"
#include "param.h"
#include "world.h"

void render(const camera& cam, const hittable& world) {
    std::ofstream image("Image.ppm");
    if (!image) { return; }

    // Render
    image << "P3\n" << cam.getImage_width() << ' ' << cam.getImage_height() << "\n255\n";
    for (int j = 0; j < cam.getImage_height(); j++) {
        std::clog << "\rScanlines remaining: " << (cam.getImage_height() - j) << ' ' << std::flush;
        for (int i = 0; i < cam.getImage_width(); i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < cam.getSamples_per_pixel(); sample++) {
                ray r = cam.get_ray(i, j);
                pixel_color += cam.ray_color(r, cam.getMax_depth(), world);
            }
            write_color(image, pixel_color * cam.getPixel_samples_scale());
        }
    }
    std::clog << "\rDone.                   \n";
}
int main() {
	// Create the World and place the objects.
	hittable_list world = create_world();
    // Initiate camera.
    camera cam;
    // Render Initialize
    int num_pixels = cam.getImage_width() * cam.getImage_height();


	render(cam, world);

	return 0;
}