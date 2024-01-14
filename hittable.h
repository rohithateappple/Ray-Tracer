#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "utils.h"

class material;

class hit_record {

	public:
		point3 p;
		vec3 normal;
		shared_ptr<material> mat;
		double t;
		bool front_face;

		void set_face_normal(const ray& r, const vec3& outward_normal) {

			auto out_norm = outward_normal;
			front_face = dot(r.direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -out_norm; // using out_norm here cuz constants cannot be negated :(
		}

};

class hittable {
	public:

		virtual ~hittable() = default;

		virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};

#endif