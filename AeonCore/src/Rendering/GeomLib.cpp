#include "acpch.h"
/*!***************************************************************************
	\file			GeomLib.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of a library of geometric
					functions and algorithms for 3D geometry calculations. It
					provides a set of functions for various geometric
					operations, including distance calculations, containment
					checks, intersection checks, angle calculations, and
					checks for parallelism and perpendicularity between
					geometric entities. These functions are designed to
					work with 3D vectors, points, lines, planes, triangles,
					spheres, and boxes. 
		
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Rendering/GeomLib.h"

namespace AeonCore
{
	/*--------------------------------------------------------------------------------------
	 *	Distance Calculations Functions
	 -------------------------------------------------------------------------------------*/
	 //	Return distance from a point to a line
	float Distance(const Point3D& point, const Line3D& line)
	{
		// Calculate the vector from the line's point to the given point.
		Vector3D w = point - line.point;

		// Calculate the cross product of the line's vector and the vector 'w'.
		Vector3D cross_product = glm::cross(line.vector, w);

		// Calculate the length of the line's vector.
		float line_length = glm::length(line.vector);

		// Calculate the distance using the formula: |crossProduct| / |line.vector|.
		float distance = length(cross_product) / line_length;

		// Return the calculated distance.
		return distance;
	}

	// Return the distance from a point to a plane.
	float Distance(const Point3D& point, const Plane3D& plane)
	{
		// Calculate the numerator by evaluating the plane equation at the given point and taking the absolute value.
		const float numerator = std::abs(plane.Evaluate(point));

		// Calculate the denominator by taking the square root of the sum of the squares of the plane's coefficients.
		const auto denominator = static_cast<float>(std::sqrt(std::pow(plane[0], 2) + std::pow(plane[1], 2) + std::pow(plane[2], 2)));

		// Calculate the distance by dividing the numerator by the denominator.
		const float distance = numerator / denominator;

		// Return the calculated distance.
		return distance;
	}

	/*--------------------------------------------------------------------------------------
	 *	Contain Functions
	 -------------------------------------------------------------------------------------*/
	 // Determines if point (known to be on a line) is contained within a segment.
	bool Segment3D::contains(const Point3D& point) const
	{
		// Check if the point's x-coordinate is within the range of the segment's x-coordinates.
		if (point.x >= std::min(point1.x, point2.x) && point.x <= std::max(point1.x, point2.x) &&
			// Check if the point's y-coordinate is within the range of the segment's y-coordinates.
			point.y >= std::min(point1.y, point2.y) && point.y <= std::max(point1.y, point2.y) &&
			// Check if the point's z-coordinate is within the range of the segment's z-coordinates.
			point.z >= std::min(point1.z, point2.z) && point.z <= std::max(point1.z, point2.z))
		{
			return true;  // The point is within the segment.
		}

		return false;  // The point is not within the segment.
	}

	// Determines if point (known to be on a line) is contained within a ray.
	bool Ray3D::contains(const Point3D& point, float* t) const
	{
		// Calculate the vector from the ray's origin to the given point.
		Vector3D v = point - origin;

		// Calculate the dot product between the vector V and the ray's direction.
		float dot_product = glm::dot(v, direction);

		// Check if the point is in front of or on the ray.
		if (dot_product >= 0)
		{
			if (t)
			{
				// Calculate the parameter 't' for each coordinate component.
				float tx = (point.x - origin.x) / direction.x;
				float ty = (point.y - origin.y) / direction.y;
				float tz = (point.z - origin.z) / direction.z;

				// Check if the differences between 't' values are very small (within tolerance).
				if (static_cast<double>(glm::abs(tx - ty)) < 10e-6 &&
					static_cast<double>(glm::abs(ty - tz)) < 10e-6 &&
					static_cast<double>(glm::abs(tz - tz)) < 10e-6)
				{
					*t = tx;  // Assign 't' value to the output parameter.
					return true;  // The point is contained within the ray.
				}
			}
		}

		return false;  // The point is not contained within the ray.
	}

	// Determines if point is contained within a box.
	bool Box3D::contains(const Point3D& point) const
	{
		// Calculate the box's corner points.
		Point3D p1 = center + extents;
		auto p2 = Point3D(center.x + extents.x, center.y + extents.y, center.z - extents.z);
		auto p3 = Point3D(center.x - extents.x, center.y + extents.y, center.z + extents.z);
		auto p4 = Point3D(center.x + extents.x, center.y - extents.y, center.z + extents.z);

		// Calculate the box's edge vectors.
		Vector3D u = p2 - p1;
		Vector3D v = p3 - p1;
		Vector3D w = p4 - p1;

		// Calculate dot products between the edge vectors and the corner points.
		float up1 = glm::dot(u, p1), up2 = glm::dot(u, p2);
		float vp1 = glm::dot(v, p1), vp2 = glm::dot(v, p3);
		float wp1 = glm::dot(w, p1), wp2 = glm::dot(w, p4);

		// Calculate dot products between the edge vectors and the given point.
		float ux = glm::dot(u, point);
		float vx = glm::dot(v, point);
		float wx = glm::dot(w, point);

		// Check if the point is within the box based on dot product comparisons.
		if ((up1 <= ux && ux <= up2) &&
			(vp1 <= vx && vx <= vp2) &&
			(wp1 <= wx && wx <= wp2))
		{
			return true;  // The point is contained within the box.
		}
		else
		{
			return false;  // The point is not contained within the box.
		}
	}

	// Determines if point (known to be on a plane) is contained within a triangle.
	bool Triangle3D::contains(const Point3D& point) const
	{
		// Assign points A, B, and C from the triangle's points array.
		Point3D a = points[0];
		Point3D b = points[1];
		Point3D c = points[2];

		// Calculate the normal vector of the triangle.
		Vector3D n = glm::cross((b - a), (c - a));

		// Perform edge tests to check if the point is within the triangle.
		if (glm::dot((point - a), (glm::cross(n, (b - a)))) >= 0 &&
			glm::dot((point - b), (glm::cross(n, (c - b)))) >= 0 &&
			glm::dot((point - c), (glm::cross(n, (a - c)))) >= 0)
		{
			return true;  // The point is contained within the triangle.
		}
		else
		{
			return false;  // The point is not contained within the triangle.
		}
	}

	/*--------------------------------------------------------------------------------------
	 *	Intersects Functions
	 -------------------------------------------------------------------------------------*/
	 // Determines if 3D line and plane have a unique intersection.  
 // If true and t is not NULL, returns intersection parameter.
	bool Intersects(const Line3D& line, const Plane3D& plane, float* rt)
	{
		// Calculate the dot products and constants.
		const float apx = plane[0] * line.point.x;
		const float bpy = plane[1] * line.point.y;
		const float cpz = plane[2] * line.point.z;

		const float avx = plane[0] * line.vector.x;
		const float bvy = plane[1] * line.vector.y;
		const float cvz = plane[2] * line.vector.z;

		const float d = plane[3];

		// Calculate the numerator and denominator for the intersection parameter.
		const float num = -apx - bpy - cpz - d;
		const float dem = avx + bvy + cvz;

		// Check if the line is not parallel to the plane (denominator is not zero).
		if (dem != 0.f)
		{
			// Calculate the intersection parameter.
			const float t = num / dem;

			if (rt)
			{
				*rt = t;  // Set the intersection parameter.

				return true;  // The line and plane have a unique intersection.
			}
		}

		return false;  // The line and plane do not have a unique intersection.
	}

	// Determines if 3D segment and plane have a unique intersection.  
	// If true and rt is not NULL, returns intersection parameter.
	bool Intersects(const Segment3D& seg, const Plane3D& plane, float* rt)      // help :(
	{
		// Calculate the vector representing the segment.
		const Vector3D vector = seg.point2 - seg.point1;

		// Evaluate the plane at the starting point of the segment.
		const float eval = plane.Evaluate(seg.point1);

		// Calculate the intersection parameter.
		const float t = -eval / dot(vector, plane.normal());

		// Check if the intersection parameter is within the valid range.
		if (t >= 0.0f && t <= 1.0f)
		{
			if (rt != nullptr)
				*rt = t;  // Set the intersection parameter.

			return true;  // The segment and plane have a unique intersection.
		}

		return false;  // The segment and plane do not have a unique intersection.
	}

	// Determines if 3D segment and triangle have a unique intersection.  
	// If true and rt is not NULL, returns intersection parameter.
#define EPSILON 0.000001f
	bool Intersects(const Segment3D& seg, const Triangle3D& tri, float* rt)
	{
		const Point3D& v0 = tri[0];
		const Point3D& v1 = tri[1];
		const Point3D& v2 = tri[2];

		const Vector3D e1 = v1 - v0;
		const Vector3D e2 = v2 - v0;

		// Calculate the direction vector of the segment.
		const Vector3D D = seg.point2 - seg.point1;

		// Set the starting point of the segment.
		const Point3D Q = seg.point1;

		const Vector3D p = cross(D, e2);
		const float d = dot(p, e1);

		if (fabs(d) < EPSILON)
			return false; // No intersection, the segment is parallel to the triangle.

		const Vector3D S = Q - v0;
		const float u = dot(p, S) / d;

		if (u < 0 || u > 1)
			return false; // No intersection, the segment intersects the plane but is outside the E2 edge.

		const Vector3D q = cross(S, e1);
		const float v = dot(D, q) / d;

		if (v < 0 || u + v > 1)
			return false; // No intersection, the segment intersects the plane but is outside the outer edges.

		const float t = dot(e2, q) / d;

		if (t < 0)
			return false; // No intersection, the segment's negative half intersects the triangle.

		if (rt != nullptr)
			*rt = t; // Set the intersection parameter.

		return true; // The segment and triangle have a unique intersection.
	}

	// Determines if 3D ray and sphere intersect.  
	// If so and rt is not NULL, returns intersection parameter.
	bool Intersects(const Ray3D& ray, const Sphere3D& sphere, float* rt)
	{
		// Calculate the parameters of the quadratic equation
		Vector3D oc = ray.origin - sphere.center;
		float a = dot(ray.direction, ray.direction);
		float b = 2 * dot(oc, ray.direction);
		float c = dot(oc, oc) - (sphere.radius * sphere.radius);

		// Calculate the discriminant
		float discriminant = b * b - 4 * a * c;

		// Check if the discriminant is negative, indicating no intersection
		if (discriminant < 0)
			return false;
		else if (discriminant <= 0)
		{
			// Only one intersection point, return true
			if (rt != nullptr)
				*rt = -b / (2 * a);
			return true;
		}
		else if (discriminant > 0)
		{
			// Calculate the two intersection points
			float t0 = (-b + sqrt(discriminant)) / (2 * a);
			float t1 = (-b - sqrt(discriminant)) / (2 * a);

			// Check if both intersection points are in front of the ray origin
			if (t0 > 0 && t1 > 0)
			{
				// Store the smallest positive intersection parameter in the provided pointer if it's not null
				if (rt != nullptr)
					*rt = std::min(t0, t1);
				return true;
			}
		}

		return false;
	}

	// Determines if 3D ray and triangle have a unique intersection.  
	// If true and rt is not NULL, returns intersection parameter.
	bool Intersects(const Ray3D& ray, const Triangle3D& tri, float* rt)
	{
		// Compute triangle edge vectors
		Vector3D edge1 = tri[1] - tri[0];
		Vector3D edge2 = tri[2] - tri[0];

		// Compute the cross product of the ray direction and edge2
		Vector3D pvec = cross(ray.direction, edge2);

		// Compute the determinant
		float det = dot(edge1, pvec);

		// Check if the ray is parallel to the triangle
		if (fabs(det) < EPSILON)
			return false;

		float inv_det = 1.0f / det;

		// Compute the vector from the ray origin to the first vertex of the triangle
		Vector3D tvec = ray.origin - tri[0];

		// Compute the u parameter
		float u = dot(tvec, pvec) * inv_det;

		// Check if the u parameter is outside the range [0, 1]
		if (u < 0.0f || u > 1.0f)
			return false;

		// Compute the qvec
		Vector3D qvec = cross(tvec, edge1);

		// Compute the v parameter
		float v = dot(ray.direction, qvec) * inv_det;

		// Check if the v parameter is outside the range [0, 1]
		if (v < 0.0f || u + v > 1.0f)
			return false;

		// Compute the intersection parameter t
		float t = dot(edge2, qvec) * inv_det;

		// Check if the intersection parameter t is positive
		if (t > 0.0f)
		{
			// Store the intersection parameter in the provided pointer if it's not null
			if (rt != nullptr)
				*rt = t;
			return true;
		}

		return false;
	}

	// Determines if 3D ray and AABB intersect.  
	// If so and rt is not NULL, returns intersection parameter.
	bool Intersects(const Ray3D& ray, const Box3D& box, float* rt)
	{
		// Initialize the intersection parameters
		float t0 = -std::numeric_limits<float>::infinity();
		float t1 = std::numeric_limits<float>::infinity();

		// Calculate the minimum and maximum bounds of the box
		Point3D box_min = box.center - box.extents;
		Point3D box_max = box.center + box.extents;

		for (glm::length_t i{}; i < 3; ++i)
		{
			if (ray.direction[i] == 0.f)
			{
				// Ray is parallel to the slab
				if (ray.origin[i] < box_min[i] || ray.origin[i] > box_max[i])
					return false;
			}
			else
			{
				// Calculate the intersection distances along the ray for the current slab
				float inv_dir = 1.f / ray.direction[i];
				float s0 = (box_min[i] - ray.origin[i]) * inv_dir;
				float s1 = (box_max[i] - ray.origin[i]) * inv_dir;

				// Ensure s0 is smaller than s1
				if (s0 > s1)
					std::swap(s0, s1);

				// Update the intersection parameter ranges
				t0 = std::max(t0, s0);
				t1 = std::min(t1, s1);

				// Check for no intersection
				if (t0 > t1)
					return false;
			}
		}

		// Check the validity of the intersection parameter range
		if (t1 < 0.f)
			return false;
		else if (t0 < EPSILON)
		{
			// Intersection at t1
			if (rt)
				*rt = t1;
			return true;
		}
		else
		{
			// Intersection at t0
			if (rt)
				*rt = t0;
			return true;
		}
	}

	// Determines if 3D triangles intersect.  
	// If parallel, returns false. (This may be considered misleading.)
	// If true and rpoint is not NULL, returns two edge/triangle intersections.
	int Intersects(const Triangle3D& tri1, const Triangle3D& tri2,
		std::pair<Point3D, Point3D>* rpoints)
	{
		std::vector<Point3D> v_point_3d;

		if (rpoints == nullptr)
			return false;

		float t1;
		for (unsigned int i{}; i < 3; ++i)
		{
			int j = (i == 2) ? 0 : i + 1;
			Segment3D seg(tri1[i], tri1[j]);

			// Check for intersection between the current segment and tri2
			if (Intersects(seg, tri2, &t1))
			{
				// Calculate the intersection point using the parameter t1
				Point3D intersection = seg.point1 + t1 * (seg.point2 - seg.point1);

				// Store the intersection point in the container
				v_point_3d.push_back(intersection);
			}
		}

		if (v_point_3d.size() == 2)
		{
			// If there are two intersection points, assign them to rpoints
			rpoints->first = v_point_3d[0];
			rpoints->second = v_point_3d[1];

			return 2;
		}

		float t2;
		for (int i = 0; i < 3; ++i)
		{
			int j = (i == 2) ? 0 : i + 1;
			Segment3D seg(tri2[i], tri2[j]);

			// Check for intersection between the current segment and tri1
			if (Intersects(seg, tri1, &t2))
			{
				// Calculate the intersection point using the parameter t2
				Point3D intersection = seg.point1 + t2 * (seg.point2 - seg.point1);

				// Store the intersection point in the container
				v_point_3d.push_back(intersection);
			}
		}

		if (v_point_3d.size() == 2)
		{
			// If there are two intersection points, assign them to rpoints
			rpoints->first = v_point_3d[0];
			rpoints->second = v_point_3d[1];

			return 2;
		}
		else
		{
			// No intersections found
			return false;
		}
	}

	bool Intersects(const Box3D& box1, const Box3D& box2, float* /*rt*/)
	{
		// Calculate the absolute vector between the centers of the two boxes
		Vector3D distance = box2.center - box1.center;
		Vector3D minSeparation = glm::abs(box1.extents + box2.extents);

		// Check for separation along each axis
		if (std::abs(distance.x) > minSeparation.x)
			return false;
		if (std::abs(distance.y) > minSeparation.y)
			return false;
		if (std::abs(distance.z) > minSeparation.z)
			return false;

		// If no separation along all axes, the boxes are intersecting
		return true;
	}

	/*--------------------------------------------------------------------------------------
	 *	Geometric Functions
	 -------------------------------------------------------------------------------------*/
	 // Compute angle between two geometric entities (in radians;  use acos)
	float AngleBetween(const Line3D& line1, const Line3D& line2)
	{
		// Calculate the dot product of the two line vectors
		const float dot_product = dot(line1.vector, line2.vector);

		// Calculate the product of the lengths of the two line vectors
		const float product_of_lengths = length(line1.vector) * length(line2.vector);

		// Calculate the angle between the lines using the dot product and product of lengths
		// Use the acos function to convert the dot product into an angle in radians
		const float angle = acos(dot_product / product_of_lengths);

		return angle;
	}

	// Compute angle between two geometric entities (in radians;  use acos)
	float AngleBetween(const Line3D& line, const Plane3D& plane)
	{
		// Calculate the dot product of the plane normal and the line vector
		const float dot_product = dot(plane.normal(), line.vector);

		// Calculate the product of the lengths of the plane normal and the line vector
		const float product_of_lengths = length(plane.normal()) * length(line.vector);

		// Calculate the angle between the line and the plane using the dot product and product of lengths
		// Use the asin function to convert the numerator into an angle in radians
		const float angle = asin(dot_product / product_of_lengths);

		return angle;
	}

	// Compute angle between two geometric entities (in radians;  use acos)
	float AngleBetween(const Plane3D& plane1, const Plane3D& plane2)
	{
		// Calculate the dot product of the normals of the two planes
		const float dot_product = dot(plane1.normal(), plane2.normal());

		// Calculate the product of the lengths of the normals of the two planes
		const float product_of_lengths = length(plane1.normal()) * length(plane2.normal());

		// Calculate the angle between the planes using the dot product and product of lengths
		// Use the acos function to convert the dot product into an angle in radians
		const float angle = acos(dot_product / product_of_lengths);

		return angle;
	}

	// Determine if two vectors are parallel.
	bool Parallel(const Vector3D& v1, const Vector3D& v2)
	{
		// Calculate the cross product of the two vectors and check if its length is zero
		// If the cross product length is zero, the vectors are parallel
		// If the cross product length is non-zero, the vectors are not parallel
		return length(cross(v1, v2)) <= 0;
	}

	// Determine if two vectors are perpendicular.
	bool Perpendicular(const Vector3D& v1, const Vector3D& v2)
	{
		// Check if the dot product of the two vectors is zero
		// If the dot product is zero, the vectors are perpendicular
		// If the dot product is non-zero, the vectors are not perpendicular
		return dot(v1, v2) <= 0;
	}

	// Determine if two lines are coplanar
	bool Coplanar(const Line3D& line1, const Line3D& line2)
	{
		// Calculate the vector AB from the first point of line1 to the first point of line2
		Vector3D ab = line2.point - line1.point;

		// Calculate the cross product of the vectors of line1 and line2
		Vector3D b1_b2 = cross(line1.vector, line2.vector);

		// Calculate the dot product of AB and B1B2
		int result = static_cast<int>(dot(ab, b1_b2));

		// If the dot product is zero, the lines are coplanar
		// If the dot product is non-zero, the lines are not coplanar
		return result == 0;
	}

	// Determine if two geometric entities are parallel.
	bool Parallel(const Line3D& line1, const Line3D& line2)
	{
		// Calculate the cross product of the vectors of the two lines
		// Calculate the length of the cross product vector
		// Check if the squared length is zero
		// If the squared length is zero, the lines are parallel
		// If the squared length is non-zero, the lines are not parallel
		return pow(length(cross(line1.vector, line2.vector)), 2) <= 0;
	}

	// Determine if two geometric entities are parallel.
	bool Parallel(const Line3D& line, const Plane3D& plane)
	{
		// Check if the dot product of the line's vector and the plane's normal is zero
		// If the dot product is zero, the line and the plane are parallel
		// If the dot product is non-zero, the line and the plane are not parallel
		return dot(line.vector, plane.normal()) <= 0;
	}

	// Determine if two geometric entities are parallel.
	bool Parallel(const Plane3D& plane1, const Plane3D& plane2)
	{
		// Calculate the cross product of the plane normals
		// Calculate the length of the cross product vector
		// Check if the squared length is zero
		// If the squared length is zero, the planes are parallel
		// If the squared length is non-zero, the planes are not parallel
		return pow(length(cross(plane1.normal(), plane2.normal())), 2) <= 0;
	}

	// Determine if two geometric entities are perpendicular.
	bool Perpendicular(const Line3D& line1, const Line3D& line2)
	{
		// Check if the dot product of the vectors of the two lines is zero
		// If the dot product is zero, the lines are perpendicular
		// If the dot product is non-zero, the lines are not perpendicular
		return dot(line1.vector, line2.vector) <= 0;
	}

	// Determine if two geometric entities are perpendicular.
	bool Perpendicular(const Line3D& line, const Plane3D& plane)
	{
		// Calculate the cross product of the plane's normal and the line's vector
		// Calculate the length of the cross product vector
		// Check if the squared length is zero
		// If the squared length is zero, the line and the plane are perpendicular
		// If the squared length is non-zero, the line and the plane are not perpendicular
		return pow(length(cross(plane.normal(), line.vector)), 2) <= 0;
	}

	// Determine if two geometric entities are perpendicular.
	bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2)
	{
		// Check if the dot product of the normals of the planes is zero
		// If the dot product is zero, the planes are perpendicular
		// If the dot product is non-zero, the planes are not perpendicular
		return dot(plane1.normal(), plane2.normal()) <= 0;
	}
}
