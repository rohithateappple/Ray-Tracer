#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include "color.h"
#include "hittable.h"
#include "vec3.h"

#include "material.h"

#include <iostream>
class camera {
	public:

		double aspect_ratio = 1.0;
		int image_width = 100;
		int sample_count = 10;
		
		int max_depth = 10;

		double vfov = 50;
		point3 look_from = vec3(0, 0, -1);
		point3 look_at = vec3(0, 0, 0);
		vec3 vup = vec3(0, 1, 0);
		point3 center;

		double defocus_angle = 0; // depth of field
		double focus_dist = 10;
		
		void render(const hittable& world) {

			initialize();
			
			std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

			for (int j = 0; j < image_height; ++j) {

				std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

				for (int i = 0; i < image_width; ++i) {

					color pixel_color(1, 0, 0);

					for (int samples = 0; samples < sample_count; ++samples) {

						// ray r = get_ray(i, j);

						pixel_color += get_ray(i, j, world); //ray_color(r, max_depth, world);
					}					

					write_color(std::cout, pixel_color, sample_count);
				}
			}
			std::clog << "\rDone.                 \n";

		}

	private:

		int image_height;
		
		point3 pixel00_loc;
		vec3 pixel_delta_u;
		vec3 pixel_delta_v;

		vec3 u, v, w; // u is right of camera, v is up, w is pointing back to look_at
		vec3 defocus_disk_u;
		vec3 defocus_disk_v;

		void initialize() {

			image_height = static_cast<int>(image_width / aspect_ratio);
			image_height = (image_height < 1) ? 1 : image_height;

			center = look_from;
			
			// stuff for determining fov
			auto theta = degrees_to_radians(vfov);
			auto h = tan(theta / 2);

			auto viewport_height = 2.0 * h * focus_dist;
			auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

			// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
			w = unit_vector(look_from - look_at);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			// Calculate the vectors across the horizontal and down the vertical viewport edges.
			vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
			vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

			// Calculate the horizontal and vertical delta vectors from pixel to pixel.
			pixel_delta_u = viewport_u / image_width;
			pixel_delta_v = viewport_v / image_height;

			// Calculate the location of the upper left pixel.
			auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
			pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

			auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
			defocus_disk_u = u * defocus_radius;
			defocus_disk_v = v * defocus_radius;

		}

		color get_ray(int i, int j, const hittable& world) const {

			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto pixel_sample = pixel_center - pixel_sample_square();

			auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
			vec3 ray_direction = pixel_center - ray_origin;
			ray r;

			color trace_color(0, 0, 0);

			vec3 point_on_curve;
			vec3 prev_pos = ray_origin;
			vec3 tangent = cycloid(2, 1.2, point_on_curve, tangent, prev_pos, pixel_center);
			
			//std::cout << "tanget: " << tangent << std::endl;
			//std::cout << "point" << point_on_curve << std::endl;

			r = ray(point_on_curve, tangent);

				
			

			

			//ray_direction = pixel_sample - ray_origin;

			//return ray(ray_origin, ray_direction);

			return  ray_color(r, max_depth, world);
		}

		vec3 pixel_sample_square() const {

			auto px = -0.5 + random_double();
			auto py = -0.5 + random_double();

			return (px * pixel_delta_u) + (py * pixel_delta_v);

		}

		color ray_color(const ray& r, int depth, const hittable& world) const {
			
			hit_record rec;

			// If we've exceeded the ray bounce limit, no more light is gathered.
			if (depth <= 0)
				return color(0, 0, 0);

			if (world.hit(r, interval(0.001, infinity), rec)) {
				ray scattered;
				color attenuation;

				if (rec.mat->scatter(r, rec, attenuation, scattered)) {
					return attenuation * ray_color(scattered, depth - 1, world);
				}

				return color(0, 0, 0);
			}

			vec3 unit_direction = unit_vector(r.direction());
			auto a = 0.5 * (unit_direction.y() + 1.0);
			return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.2, 0.4, 0.8); // sky color
		}

		point3 defocus_disk_sample() const {
			
			auto p = random_unit_disk();

			return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);

		}

		vec3 cycloid(int t, double radius, vec3& position, vec3& tangent, point3 previous_position, vec3 pixel_c) const {
			
			auto x = radius * (t - std::sin(t)) + pixel_c.x();
			auto y = radius * (1.0 - std::cos(t)) + pixel_c.y();
			auto z = pixel_c.z();

			position = vec3(x, y, z);
			tangent = previous_position - position;
			previous_position = position;
			//tangent.normalize();
			
			return tangent;
		}
};

#endif
