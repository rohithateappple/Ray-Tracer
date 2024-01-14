#include "utils.h"
#include "color.h"
#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

#include <iostream>
#include <memory>


// to save run this at terminal --> & "D:\kin Programming\RayTrace test\x64\Debug\RayTrace test.exe" > image.ppm

int main() {

	hittable_list world; // stores all hittable objects

	auto material_ground = make_shared<lambertian>(color(0.7, 0.2, 0.5));
	auto material_diffuse = make_shared<lambertian>(color(0.2, 0.7, 0.5));
	auto material_diffuse_2 = make_shared<lambertian>(color(1, 0.3, 0.1));
	auto material_metallic = make_shared<metal>(color(0.9, 0.2, 0.0), 0.1);
	auto material_glass_1 = make_shared<dielectric>(1.5, color(1, 1, 1));
	auto material_glass_2 = make_shared<dielectric>(2.7, color(1, 1, 0.2));

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_diffuse));
	world.add(make_shared<sphere>(point3(0.0, 0.0, 1.0), 0.5, material_diffuse_2));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_glass_1));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.3, material_metallic));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_glass_2));

	camera cam;

	cam.aspect_ratio = 1 / 1;
	cam.image_width = 12;
	cam.sample_count = 5;
	cam.max_depth = 2;
	
	cam.vfov = 90;
	cam.look_from = point3(0, 0, 3);
	cam.look_at = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;
	cam.focus_dist = 5;

	//cam.render(world);

	for (int i = 0; i < 50; i++) {
		std::cout << random_in_unit_sphere() << std::endl;
	}

}