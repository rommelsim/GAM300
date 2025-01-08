#pragma once
/*!***************************************************************************
	\file			GeomLib.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of geometric functions and
					calculations for 3D geometry.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>

const float PI = 3.14159f;

// Forward declarations:
using Point3D = glm::vec3;
using Vector3D = glm::vec3;
using Vector4D = glm::vec4;
using Matrix4x4 = glm::mat4;

using glm::normalize;
using glm::length;
using glm::cross;
using glm::dot;

class Line3D;
class Box3D;
class Sphere3D;
class Plane3D;
class Triangle3D;

namespace AeonCore
{
	/*--------------------------------------------------------------------------------------
	 *	LINE3D
	 -------------------------------------------------------------------------------------*/
	class Line3D
	{
	public:
		// Constructors
		Line3D() : point(), vector() { return; }
		Line3D(const Point3D& p, const Vector3D& v) : point(p), vector(v) { return; }
		Point3D lerp(const float t) const { return(point + t * vector); }

	public:
		Point3D point;
		Vector3D vector;
	};

	// Functions
	float AngleBetween (const Line3D& line1, const Line3D& line2);
	bool  Coplanar     (const Line3D& line1, const Line3D& line2);
	bool  Parallel     (const Line3D& line1, const Line3D& line2);
	bool  Perpendicular(const Line3D& line1, const Line3D& line2);

	float AngleBetween (const Line3D& line, const Plane3D& plane);
	bool  Parallel     (const Line3D& line, const Plane3D& plane);
	bool  Perpendicular(const Line3D& line, const Plane3D& plane);
	bool  Intersects   (const Line3D& line, const Plane3D& plane, float* rt = NULL);

	/*--------------------------------------------------------------------------------------
	 *	SEGMENT3D
	 -------------------------------------------------------------------------------------*/
	class Segment3D
	{
	public:
		// Constructors
		Segment3D() : point1(), point2() { return; }
		Segment3D(const Point3D& p1, const Point3D& p2) : point1(p1), point2(p2) { return;}
		Point3D lerp(const float t) const { return((1.0f - t) * point1 + t * point2); }

		// Utility methods
		bool contains(const Point3D& point) const;

		Point3D point1;
		Point3D point2;
	};

	// Functions
	bool Intersects(const Segment3D& seg, const Triangle3D& tri, float* rt = NULL);

	/*--------------------------------------------------------------------------------------
	 *	RAY3D
	 -------------------------------------------------------------------------------------*/
	class Ray3D
	{
	public:
		// Constructor
		Ray3D() : origin(), direction() { return; }
		Ray3D(const Point3D& o, const Vector3D& d) : origin(o), direction(d) { return;}
		Point3D lerp(const float t) const { return(origin + t * direction); }


		// Containment method
		bool contains(const Point3D& point, float* t = NULL) const;
		// Returns paramter of intersection if containment is true and t != NULL

		Point3D  origin;
		Vector3D direction;
	};

	// Utility functions:
	bool Intersects(const Ray3D& ray, const Sphere3D& sphere, float* rt = NULL);
	bool Intersects(const Ray3D& ray, const Triangle3D& tri, float* rt = NULL);
	bool Intersects(const Ray3D& ray, const Box3D& box, float* rt = NULL);

	/*--------------------------------------------------------------------------------------
	 *	BOX3D
	 -------------------------------------------------------------------------------------*/
	class Box3D
	{
	public:
		// Constructor
		Box3D() { return; }
		Box3D(const Point3D& c, const Vector3D& e) : center(c), extents(e) { return; }

		// Utility method
		bool contains(const Point3D& point) const;
		// Custom abs function for a Vector3D
		

		Point3D  center;    // Center point
		Vector3D extents;   // Center to corner half extents.
	};
	// Utility functions:

	/*--------------------------------------------------------------------------------------
	 *	SPHERE3D
	 -------------------------------------------------------------------------------------*/
	class Sphere3D
	{
	public:
		// Constructors
		Sphere3D() : center(), radius(0) { return; }
		Sphere3D(const Point3D& c, const float r) : center(c), radius(r) { return; }

		Point3D center;
		float radius;
	};
	bool Intersects(const Box3D& box1, const Box3D& box2, float* rt = NULL);
	/*--------------------------------------------------------------------------------------
	 *	PLANE3D
	 -------------------------------------------------------------------------------------*/
	class Plane3D
	{
	public:
		// Constructor
		Plane3D(const float A = 0, const float B = 0, const float C = 0, const float D = 0)
		{
			crds[0] = A; crds[1] = B; crds[2] = C; crds[3] = D;
		}

		// Indexing operators.
		float&       operator[](const unsigned int i)       { return crds[i]; }
		const float& operator[](const unsigned int i) const { return crds[i]; }

		// Utility methods.
		Vector3D normal() const            { return Vector3D(crds[0], crds[1], crds[2]); }
		float    Evaluate(Point3D p) const { return crds[0] * p[0] + crds[1] * p[1] + crds[2] * p[2] + crds[3]; }

	private:
		enum { DIM = 4 };
		float crds[DIM];

	};

	// Utility functions:
	float AngleBetween(const Plane3D& plane1, const Plane3D& plane2);
	bool  Parallel(const Plane3D& plane1, const Plane3D& plane2);
	bool  Perpendicular(const Plane3D& plane1, const Plane3D& plane2);
	bool  Intersects(const Segment3D& seg, const Plane3D& plane, float* rt = NULL);

	/*--------------------------------------------------------------------------------------
	 *	TRIANGLE3D
	 -------------------------------------------------------------------------------------*/
	class Triangle3D
	{
	public:
		// Constructor
		Triangle3D() { return; }
		Triangle3D(const Point3D& p1, const Point3D& p2, const Point3D& p3)
		{
			points[0] = p1; points[1] = p2; points[2] = p3;
		}

		Point3D& operator[](unsigned int i) { return points[i]; }
		const Point3D& operator[](unsigned int i) const { return points[i]; }


		bool contains(const Point3D& point) const;

	private:
		enum { DIM = 3 };
		Point3D points[DIM];
	};

	// Utility function:
	int Intersects(const Triangle3D& tri1, const Triangle3D& tri2, std::pair<Point3D, Point3D>* rpoints = 0);
}