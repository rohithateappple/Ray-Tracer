#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"

class hit_record;

class material {

	public:
		virtual ~material() = default;
		virtual bool scatter(const ray& ray_in, const hit_record& rec, 
							color& attenuation, ray& scattered_ray) const = 0;
};

// using lambertian diffuse. for regular use: random_on_hemisphere(rec.normal)

class lambertian : public material {

	public:
		lambertian(const color& a) : albedo(a) {}
		
		bool scatter(const ray& ray_in, const hit_record& rec,
			color& attenuation, ray& scattered_ray) const override {

			auto scatter_direction = rec.normal + random_unit_vector();
			
			// catch direction values less than zero which might result in NaNs
			
			if (scatter_direction.near_zero()) {
				scatter_direction = rec.normal;
			};
			
			scattered_ray = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}

	private:
		color albedo;

};

class metal : public material {

	public:
		metal(const color& a, const double _fuzz) : albedo(a), fuzz(_fuzz) {}

		bool scatter(const ray& ray_in, const hit_record& rec,
			
			color& attenuation, ray& scattered_ray) const override {

			auto reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
			
			scattered_ray = ray(rec.p, reflected + fuzz * random_unit_vector());
			attenuation = albedo;
			
			// checking if the scattered ray is in the same hemisphere as normal

			return (dot(scattered_ray.direction(), rec.normal) > 0); 
		}		

	private:
		color albedo;
		double fuzz; // meaning roughness here.
		
};

class dielectric : public material {

	public:

		dielectric(const double index_of_refraction, const color& color) : ior(index_of_refraction), glass_color(color) {}
	
		bool scatter(const ray& ray_in, const hit_record& rec,

			color& attenuation, ray& scattered_ray) const override {
		
			attenuation = glass_color;
			double refraction_ratio = rec.front_face ? (1.0 / ior) : ior; // check if we're inside or outside an object

			vec3 unit_direction = unit_vector(ray_in.direction());
			double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			vec3 direction;

			if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
				direction = reflect(unit_direction, rec.normal);
			else
				direction = refract(unit_direction, rec.normal, refraction_ratio);

			scattered_ray = ray(rec.p, direction);

			return true;
		
		}

	private:
		double ior;
		color glass_color;

		static double reflectance(double cosine, double ref_idx) {
			
			// Use Schlick's approximation for reflectance.
			auto r0 = (1 - ref_idx) / (1 + ref_idx);
			r0 = r0 * r0;
			
			return r0 + (1 - r0) * pow((1 - cosine), 5);
		}

		


};

#endif 

