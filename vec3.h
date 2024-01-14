#pragma once

/* Defining a Vector3 class */

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
	public:
		double e[3]; // class stores three type doubles 

		vec3() : e{ 0.0, 0.0, 0.0 } {} // constructor initializing to 0 
	
		/* constructor to initialize vector with values */
		vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} 

		/* simple getter functions */
		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		/* negation operator function, to turn the vector negative */
		vec3 operator- () { return vec3(-e[0], -e[1], -e[2]); }

		/* indexing operator function, to get element of the vector */
		double operator[] (int i) const { return e[i]; }

		/* indexing operator function, returns reference to the element. allows modification */
		double& operator[] (int i) { return e[i]; }
		
		/* addition operator function*/
		vec3& operator+=(const vec3& v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];

			/* Note: returning *this allows for compound operations like a += b += c
			   In this contex *this represents the input itself, so the modified input is returned */

			return *this; 
		}

		/* multiplier operator function*/
		vec3& operator*=(double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		/* division operator function */
		vec3& operator/=(double t) {

			/* Note: the below is same as 4 / 2 = 2 also written as 4 * (1 / 2) = 2 */
			return *this *= 1 / t;
		}

		/* The two functions below allows us to calculate the distances between vectors. e.g:
		   
		   vec3 A = (1, 2, 1);
		   vec3 B = (5, 2, 2);
		   vec3 difference = A - B;
		   double distance = difference.length()

		*/

		/* get the sqrt of the distance between the vector and the origin */
		double length() const {
			return sqrt(length_squared());
		}

		/* get the squared distance between the vector and the origin */
		double length_squared() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}	
		
		static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		static vec3 random(double min, double max) {
			return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

		bool near_zero() {
			auto s = 1e-8;
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		}

		void normalize() {
			double length = std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
			if (length != 0.0) {
				e[0] /= length;
				e[1] /= length;
				e[2] /= length;
			}
		}

};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << '{' << v.e[0] << ',' << ' ' << v.e[1] << ',' << ' ' << v.e[2] << '}';
}

/* standard math operations for two vectors */

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

/* more complicated math operations */

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

// sample a point in a cube between -1 & 1. If it's squared length is < 1 then it's in a sphere.

inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() < 1)
			return p;
	}
}

// normalize the unit in sphere so its on the surface

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

// check if the unit vector is pointing in the proper direction by taking dot product of normal

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

// calculate the angle of reflection for polished surface

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(vec3& uv, const vec3& n, double etai_over_etat) {
	
	auto cos_theta = fmin(dot(-uv, n), 1.0); // calculating the incident angle 
	
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	
	return r_out_perp + r_out_parallel;
}

inline vec3 random_unit_disk() {
	
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);

		if (p.length_squared() < 1) {
			return p;
		}
	}
	
}


inline vec3 generate_sine_wave_direction(const vec3& origin, const vec3& target, double t, double frequency, double amplitude) {
	// Calculate the vector from origin to target
	vec3 direction_to_target = target - origin;

	// Modify the direction based on a sine wave along each axis
	vec3 sine_wave_direction;
	auto x = direction_to_target.x() + amplitude * std::sin(frequency * t);
	auto y = direction_to_target.y() + amplitude * std::sin(frequency * t);
	auto z = direction_to_target.z() + amplitude * std::sin(frequency * t);

	return sine_wave_direction;
}


#endif