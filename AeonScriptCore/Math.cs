/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe 
 * file:	Math.cs
 * authors:
		Lam En Qing					(enqing.lam@digipen.edu)
 *
 * brief:	Defines the Vector2 and Vector3 structs, as well as
            a MathHelper class that contains Math functions for
            the float type.
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
-----------------------------------------------------------*/
using System;

namespace AeonScriptCore
{
    public struct Vector2
    {
        public float X, Y;

        public Vector2(float value)
        {
            X = Y = value;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public override string ToString()
        {
            return "[X:" + X + ", Y:" + Y + "]";
        }

        // Applying negative or minus sign to a vector
        public static Vector2 operator-(Vector2 vec) => new Vector2(-vec.X, -vec.Y);

        // Between 2 vectors or a vector & a float
        public static Vector2 operator+(Vector2 v1, Vector2 v2) => new Vector2(v1.X + v2.X, v1.Y + v2.Y);
        public static Vector2 operator+(float val, Vector2 vec) => new Vector2(val + vec.X, val + vec.Y);
        public static Vector2 operator+(Vector2 vec, float val) => new Vector2(val + vec.X, val + vec.Y);

        public static Vector2 operator-(Vector2 v1, Vector2 v2) => new Vector2(v1.X - v2.X, v1.Y - v2.Y);
        public static Vector2 operator-(float val, Vector2 vec) => new Vector2(val - vec.X, val - vec.Y);
        public static Vector2 operator-(Vector2 vec, float val) => new Vector2(val - vec.X, val - vec.Y);

        public static Vector2 operator*(float val, Vector2 vec) => new Vector2(val * vec.X, val * vec.Y);
        public static Vector2 operator*(Vector2 vec, float val) => new Vector2(val * vec.X, val * vec.Y);

        public static Vector2 operator/(Vector2 vec, float val) => new Vector2(vec.X / val, vec.Y / val);
    }

    public struct Vector3
    {
        public static Vector3 ZERO = new Vector3(0);
        public static Vector3 ONE = new Vector3(1);

        public float X, Y, Z;

        public Vector3(float value)
        {
            X = Y = Z = value;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public override string ToString()
        {
            return "[X:" + X + ", Y:" + Y + ", Z:" + Z + "]";
        }

        public float GetMagnitudeSq()
        {
            return X * X + Y * Y + Z * Z;
        }

        public float GetMagnitude()
        {
            return MathHelper.Sqrt(X * X + Y * Y + Z * Z);
        }

        // Applying negative or minus sign to a vector
        public static Vector3 operator-(Vector3 vec) => new Vector3(-vec.X, -vec.Y, -vec.Z);

        /// <summary>
        /// Cross product using the right hand rule
        /// </summary>
        /// <param name="a">The vector on the right side, also on the index finger</param>
        /// <param name="b">The vector on the left side, also on the middle finger</param>
        /// <returns>The vector of the upward direction, also on the thumb finger</returns>
        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X);
        }
        
        /// <summary>
        /// Get the normalized vector of a given vector
        /// </summary>
        /// <param name="vector">The vector you wish to normalize</param>
        /// <returns></returns>
        public static Vector3 Normalize(Vector3 vector)
        {
            float magnitude = vector.GetMagnitude();
            if (magnitude > 0)
            {
                return new Vector3(vector.X / magnitude, vector.Y / magnitude, vector.Z / magnitude);
            }
            return Vector3.ZERO;
        }

        /// <summary>
        /// Get the squared distance between 2 points in 3D space
        /// </summary>
        /// <param name="a">The first point</param>
        /// <param name="b">The second point</param>
        /// <returns></returns>
        public static float DistSq(Vector3 a, Vector3 b)
        {
            float xDiff = a.X - b.X;
            float yDiff = a.Y - b.Y;
            float zDiff = a.Z - b.Z;
            return xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
        }

        /// <summary>
        /// Get the squared distance between 2 points in 3D space
        /// </summary>
        /// <param name="a">The first point</param>
        /// <param name="b">The second point</param>
        /// <returns></returns>
        public static float Dist(Vector3 a, Vector3 b)
        {
            return MathHelper.Sqrt(DistSq(a, b));
        }

        public static Vector3 Lerp(Vector3 start, Vector3 end, float time)
        {
            return start + (end - start) * time;
        }

        public static Vector3 operator+(Vector3 v1, Vector3 v2) => new Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
        public static Vector3 operator+(float val, Vector3 vec) => new Vector3(vec.X + val, vec.Y + val, vec.Z + val);
        public static Vector3 operator+(Vector3 vec, float val) => new Vector3(vec.X + val, vec.Y + val, vec.Z + val);

        public static Vector3 operator-(Vector3 v1, Vector3 v2) => new Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
        public static Vector3 operator-(float val, Vector3 vec) => new Vector3(vec.X - val, vec.Y - val, vec.Z - val);
        public static Vector3 operator-(Vector3 vec, float val) => new Vector3(vec.X - val, vec.Y - val, vec.Z - val);

        public static Vector3 operator*(float val, Vector3 vec) => new Vector3(vec.X * val, vec.Y * val, vec.Z * val);
        public static Vector3 operator*(Vector3 vec, float val) => new Vector3(vec.X * val, vec.Y * val, vec.Z * val);

        public static Vector3 operator/(Vector3 vec, float val) => new Vector3(vec.X / val, vec.Y / val, vec.Z / val);

        public static bool operator==(Vector3 leftVec, Vector3 rightVec)
        {
            return leftVec.X == rightVec.X && leftVec.Y == rightVec.Y && leftVec.Z == rightVec.Z;
        }
        public static bool operator!=(Vector3 leftVec, Vector3 rightVec)
        {
            return !(leftVec == rightVec);
        }
    }

    /// <summary>
    /// Math helper functions for float type
    /// </summary>
    public class MathHelper
    {
        public static readonly float PI = (float)Math.PI;
        public static readonly float PI_Half = PI / 2.0f;
        public static readonly float RadToDeg = 180.0f / PI;
        public static readonly float DegToRad = PI / 180.0f;

        /// <summary>
        /// Make the given value an absolute, that is it becomes a positive number
        /// </summary>
        /// <param name="value">The value to make absolute</param>
        /// <returns>A float</returns>
        public static float Abs(float value)
        {
            return value >= 0 ? value : -value;
        }

        /// <summary>
        /// Returns the square root of a float value
        /// </summary>
        /// <param name="value">The float value to square root</param>
        /// <returns>A float value</returns>
        public static float Sqrt(float value)
        {
            return InternalCalls.Math_Sqrt(value);
        }

        public static float Round(float value, int places)
        {
            decimal input = (decimal)value;
            input = decimal.Round(input, places);
            return (float)input;
        }

        /// <summary>
        /// Returns the sine of the angle
        /// </summary>
        /// <param name="radAngle">Angle value in radians</param>
        /// <returns>A float value</returns>
        public static float Sin(float radAngle)
        {
            return InternalCalls.Math_Sin(radAngle);
        }

        /// <summary>
        /// Returns the cosine of the angle
        /// </summary>
        /// <param name="radAngle">Angle value in radians</param>
        /// <returns>A float value</returns>
        public static float Cos(float radAngle)
        {
            return InternalCalls.Math_Cos(radAngle);
        }

        /// <summary>
        /// Returns the tangent of the angle
        /// </summary>
        /// <param name="radAngle">Angle value in radians</param>
        /// <returns>A float value</returns>
        public static float Tan(float radAngle)
        {
            return InternalCalls.Math_Tan(radAngle);
        }

        /// <summary>
        /// Returns the arcsine of the angle, in radians
        /// </summary>
        /// <param name="value">The division value of opposite and hypotenuse lengths</param>
        /// <returns>A float value</returns>
        public static float ASin(float value)
        {
            return InternalCalls.Math_ASin(value);
        }

        /// <summary>
        /// Returns the arccosine of the angle, in radians
        /// </summary>
        /// <param name="value">The division value of adjacent and hypotenuse lengths</param>
        /// <returns>A float value</returns>
        public static float ACos(float value)
        {
            return InternalCalls.Math_ACos(value);
        }

        /// <summary>
        /// Returns the arctangent of the angle, in radians
        /// </summary>
        /// <param name="value">The division value of opposite and adjacent lengths</param>
        /// <returns>A float value</returns>
        public static float ATan(float value)
        {
            return InternalCalls.Math_ATan(value);
        }

        public static float ATan2(float x, float y)
        {
            return InternalCalls.Math_ATan2(x,y);
        }

        /// <summary>
        /// Clamps the given value between a minimum and maximum value. If the value is in range, the value will be returned.
        /// </summary>
        /// <param name="value">The value you wish to clamp to</param>
        /// <param name="min">The minimum value</param>
        /// <param name="max">The maximum value</param>
        /// <returns></returns>
        public static float Clamp(float value, float min, float max) {
            if (value < min) { return min; }
            else if (value > max) { return max; }
            return value;
        }
    }
}
