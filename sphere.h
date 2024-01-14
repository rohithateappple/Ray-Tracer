#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
	
	public:
		sphere(point3 _center, double _radius, shared_ptr<material> _material, bool _visible = true) : center(_center), radius(_radius), mat(_material), visible(_visible) {}

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
			
			if(visible)
			{
				vec3 oc = r.origin() - center;
				auto a = r.direction().length_squared();
				auto half_b = dot(oc, r.direction());
				auto c = dot(oc, oc) - radius * radius;

				auto discriminant = half_b * half_b - a * c;
				if (discriminant < 0) return false; // if nothing hits return false
				auto sqrtd = sqrt(discriminant);

				// checking for hits within a given distance of the ray. ray_tmin and tmax define the length of line
				auto root = (-half_b - sqrtd) / a;
				
				if (!ray_t.surrounds(root)) {
					root = (-half_b + sqrtd) / a;
					if (!ray_t.surrounds(root))
						return false;
				}

				rec.t = root;
				rec.p = r.at(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.set_face_normal(r, rec.normal);
				rec.mat = mat;

				return true;
			
			}

			return false;

			
		}

	private:
		point3 center;
		double radius;
		shared_ptr<material> mat;
		bool visible;
};

#endif