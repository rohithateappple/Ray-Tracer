#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "ray.h"
#include <memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;


class hittable_list : public hittable {

	public:

		// basically an unbound array to store hittables

		std::vector<std::shared_ptr<hittable>> objects;

		hittable_list() {};
		hittable_list(shared_ptr<hittable> object) { add(object); }

		void clear() {
			objects.clear();
		}

		void add(shared_ptr<hittable> object) {
			objects.push_back(object);
		}


		// going through each object in the array and checking for hits.
		bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

			hit_record temp_rec;
			bool hit_anything = false;
			auto closest_so_far = ray_t.max;

			for (const auto& object : objects) {
				if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
					hit_anything = true;
					closest_so_far = temp_rec.t;
					rec = temp_rec;

				}
			}
	
			return hit_anything;
		}

};

#endif