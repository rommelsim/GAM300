#pragma once
/*!***************************************************************************
	\file            ScriptUtils.h
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file contains utility function declarations as well
					 as an enum class ScriptDataType
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"

#include <mono/jit/jit.h>
#include <string>

namespace AeonCore {
	/// <summary>
	/// ScriptDataType describes the C# equivalent into C
	/// 
	/// Bool : bool
	/// Char : unsigned short (Use ScriptCTypes for the actual type)
	/// Float : float
	/// Double : double
	/// Byte : unsigned char (use ScriptCTypes for the actual type)
	/// Short : short
	/// Int : int
	/// Long : long
	/// UShort : unsigned short
	/// UInt : unsigned int
	/// ULong : unsigned long
	/// String : const char*
	/// </summary>
	enum class AEON_API ScriptDataType {
		None = 0,
		Bool,
		Float, Double,
		Int,
		String,
		/*Vector2,*/ Vector3
	};

	// Type alias primitive types that don't exactly name-match from C# to C
	/*namespace ScriptCTypes {
		using myChar = unsigned short;
		using myByte = unsigned char;
	}*/

	namespace ScriptUtils {
		/// <summary>
		/// Converts C++ string to MonoString
		/// </summary>
		/// <param name="cStr">The char* string you wish to convert into MonoString</param>
		/// <returns>A pointer to MonoString</returns>
		MonoString* CToMonoString(const char* cStr);
		/// <summary>
		/// Converts MonoString to C++ string
		/// </summary>
		/// <param name="monoString">The MonoString* string you wish to convert to std::string</param>
		/// <returns>A std::string</returns>
		std::string MonoToCString(MonoString* monoString);

		/// <summary>
		/// Gives the string name of the values inside the enum ScriptDataType
		/// </summary>
		/// <param name="dataType">The enum value to get its string name for</param>
		/// <returns>A c-style string, const char*</returns>
		AEON_API inline std::string ScriptDataTypeToString(ScriptDataType dataType) {
			switch (dataType) {
			case ScriptDataType::None: return "None";
			case ScriptDataType::Bool: return "Bool";
			case ScriptDataType::Float: return "Float";
			case ScriptDataType::Double: return "Double";
			case ScriptDataType::Int: return "Int";
			case ScriptDataType::String: return "String";
			case ScriptDataType::Vector3: return "Vector3";
			//case ScriptDataType::Char: return "Char";
			//case ScriptDataType::Byte: return "Byte";
			//case ScriptDataType::Short: return "Short";
			//case ScriptDataType::Long: return "Long";
			//case ScriptDataType::UShort: return "UShort";
			//case ScriptDataType::UInt: return "UInt";
			//case ScriptDataType::ULong: return "ULong";
			}
			return "None";
		}

		/// <summary>
		/// Gives the appropriate enum value of ScriptDataType
		/// </summary>
		/// <param name="typeString">A read-only string, the string name of the enum value you wish to get</param>
		/// <returns>The appropriate enum value if the string name matches, otherwise ScriptDataType::None is returned</returns>
		AEON_API inline ScriptDataType ScriptDataTypeFromString(std::string_view typeString) {
			if (typeString == "None") return ScriptDataType::None;
			else if (typeString == "Bool") return ScriptDataType::Bool;
			else if (typeString == "Float") return ScriptDataType::Float;
			else if (typeString == "Double") return ScriptDataType::Double;
			else if (typeString == "Int") return ScriptDataType::Int;
			else if (typeString == "String") return ScriptDataType::String;
			else if (typeString == "Vector3") return ScriptDataType::Vector3;
			//else if (typeString == "Char") return ScriptDataType::Char;
			//else if (typeString == "Byte") return ScriptDataType::Byte;
			//else if (typeString == "Short") return ScriptDataType::Short;
			//else if (typeString == "UInt") return ScriptDataType::UInt;
			//else if (typeString == "Long") return ScriptDataType::Long;
			//else if (typeString == "UShort") return ScriptDataType::UShort;
			//else if (typeString == "ULong") return ScriptDataType::ULong;
			return ScriptDataType::None;
		}

		/// <summary>
		/// A utility function to create a float array for Mono
		/// </summary>
		/// <param name="count">The number of elements inside the float array</param>
		/// <param name="dataArr">The float array itself, of type float*</param>
		/// <returns>A MonoArray*</returns>
		MonoArray* CreateMonoArrayFloat(int count, float* dataArr);

		/// <summary>
		/// A utility function to create a double array for Mono
		/// </summary>
		/// <param name="count">The number of elements inside the double array</param>
		/// <param name="dataArr">The double array itself, of type double*</param>
		/// <returns>A MonoArray*</returns>
		MonoArray* CreateMonoArrayDouble(int count, double* dataArr);

		/// <summary>
		/// A utility function to create a int array for Mono
		/// </summary>
		/// <param name="count">The number of elements inside the int array</param>
		/// <param name="dataArr">The int array itself, of type int*</param>
		/// <returns>A MonoArray*</returns>
		MonoArray* CreateMonoArrayInt(int count, int* dataArr);

		/// <summary>
		/// A utility function to create a unsigned int array for Mono
		/// </summary>
		/// <param name="count">The number of elements inside the unsigned int array</param>
		/// <param name="dataArr">The unsigned int array itself, of type unsigned int*</param>
		/// <returns>A MonoArray*</returns>
		MonoArray* CreateMonoArrayUnsignedInt(int count, uint32_t* dataArr);

		/// <summary>
		/// Execute your desired function inside the C# class
		/// </summary>
		/// <param name="classInstance">The instance of the class, it can be given a nullptr if you are running static methods.</param>
		/// <param name="method">The Mono representation of the C# method, cannot be nullptr at all</param>
		/// <param name="params">An array of void*. To add a parameter value, you must give the address of the variable representing the parameter value</param>
		/// <returns>A MonoObject*, represents the return value of the invoked C# function</returns>
		MonoObject* MonoInvokeMethod(MonoMethod* method, MonoObject* classInstance = nullptr, void** params = nullptr);
	};
}