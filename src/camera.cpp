#include "camera.hpp"

#include "color.hpp"
#include "hittable.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "ray.hpp"

void camera::render(const hittable& world) {
  initialize();

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
              << std::flush;
    for (int i = 0; i < image_width; i++) {
      auto pixel_center{pixel00_loc + i * pixel_delta_u + j * pixel_delta_v};
      auto ray_direction{pixel_center - center};
      auto r{ray(center, ray_direction)};

      auto pixel_color{ray_color(r, world)};
      write_color(std::cout, pixel_color);
    }
  }

  std::clog << "\rDone.                 \n";
}

void camera::initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    center = point(0, 0, 0);

    // Determine viewport dimensions.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

color camera::ray_color(const ray& r, const hittable& world) const {
  hit_record rec;
  if (world.hit(r, interval(0, std::numeric_limits<double>::infinity()), rec)) {
    return (rec.normal + color(1, 1, 1)) / 2;
  }

  auto unit_direction{r.direction().unit_vector()};
  auto a{(unit_direction.y() + 1.0) / 2};
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}