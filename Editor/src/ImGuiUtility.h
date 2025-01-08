#pragma once
/*!***************************************************************************
	\file			ImGuiUtility.h
	\author			Chew Shi Ru
	\par			shiru.chew@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations to
					draw ImGui function aids in the process when doing the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//External Lib
#include <document.h>
#include <xstring>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
//Engine
#include <ECS/Components/Render.h>
namespace Editor {
	class ImGuiUtility {
	public:
		ImGuiUtility() = default;
		/*!***************************************************************************
		*   \brief			This function will draw three ImGui::DragFloat on the same 
							line along with a tag to be used for eg "Transform Component"
			\param  _fn		const std::string& label, glm::vec3& values
			\return			void
		*******************************************************************************/
		static bool DrawVec3Control(const std::string& label, glm::vec3& values, float reset = 0.0f, float columnWidth = 125.0f);
		static bool DrawVec3ControlTmpForXuan(const std::string &label, glm::vec3 &values, float min, float max, float reset = 0.0f, float columnWidth = 125.0f);
		 /*!***************************************************************************
		 *   \brief			This function will draw four ImGui::DragFloat on the same
							line along with a tag"
			 \param  _fn	const std::string& label, glm::quat& values, float reset = 0.0f
			 \return		void
		 *******************************************************************************/
		static bool DrawVec4Control(const std::string& label, glm::quat values, float reset = 0.0f, float columnWidth = 125.0f);
		 /*!***************************************************************************
		 *   \brief			This function will draw general text and label frame, note 
							that value cannot be changed on the editor.
			 \param  _fn	const std::string& tag, const char* label, char* buffer, size_t bufferSize
			 \return		void
		 *******************************************************************************/
		static void DrawTextLabelControl(const std::string& tag, const char* label, char* buffer, size_t bufferSize, float columnWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw two label frame, note
						   that value cannot be changed on the editor.
			\param  _fn	const std::string& tag, const char* label, char* buffer, size_t bufferSize
			\return		void
		*******************************************************************************/
		static void DrawLabelControl(const std::string& tag, const std::string& label, float columnWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw general text and label frame, note
						   that value can be changed on the editor.
			\param  _fn	const std::string& tag, const char* label, char* buffer, size_t bufferSize
			\return		void
		*******************************************************************************/
		static bool DrawEnterInput(const std::string& tag, const char* label, std::string* text, float columnWidth = 125.0f);

		/*!***************************************************************************
		 *   \brief			This function will draw single ImGui::Dragfloat along with a tag
			 \param  _fn	const std::string& tag, const char* label, float* v
			 \return		void
		 *******************************************************************************/
		static void DrawDragFloat(const std::string& tag, const char* label, float* v, float columnWidth = 125.0f);
		static void DrawDragFloat(const std::string& tag, const char* label, float* v, float min, float max,float columnWidth = 125.0f);
		/**
		 * \brief To draw a drop drag target frame for user to drop drag objects from asset browser to the various components for eg "texture"
		 */
		static void DrawDropDragTarget(const std::string& tag, const char* label, float columnWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw ImGui::Color4
			\param  _fn		nil
			\return		    void
		*******************************************************************************/
		static void DrawColor(const std::string& tag, const char* label, float value[3], float columnWidth = 125.0f);

		static void DrawColor( const std::string& label, AeonCore::Render& renderCompo, const glm::vec3& component, float columnWidth=125.0f);
		/*!***************************************************************************
		*   \brief			This function will set the editor to a dark theme
			\param  _fn		nil	
			\return		    void
		*******************************************************************************/
		static void SetDarkThemeColor();
		///*!***************************************************************************
		//*   \brief			This function will draw checkBox along with a label
		//	\param  _fn		const std::string& tag, const char* label, bool* v
		//	\return		    void
		//*******************************************************************************/
		static bool DrawCheckBox(const std::string& tag, const char* label, bool* v, float columnWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw ImGui::InputInt along with a label
			\param  _fn		const std::string & tag, const char* label, const char* value
			\return		    bool
		*******************************************************************************/
		static bool DrawInputInt(const std::string& tag, const char* label, int* value, float columWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw ImGui::InputInt along with a label
			\param  _fn		const std::string & tag, const char* label, const char* value
			\return		    bool
		*******************************************************************************/
		static bool DrawInputInt(const std::string& tag, const char* label, unsigned int* value, float columWidth = 125.0f);

		/*!***************************************************************************
		*   \brief			This function will draw ImGui::InputFloat along with a label
			\param  _fn		const std::string& tag, const char* label, float* value, float step, float step_fast, const char* format, float coloumWidth=125.0f
			\return		    bool
		*******************************************************************************/
		static bool DrawInputFloat(const std::string& tag, const char* label, float* value, float step, float step_fast, const char* format, float coloumWidth=125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw ImGui::InputDouble along with a label
			\param  _fn		const std::string& tag, const char* label, double* value, float step, float step_fast, const char* format, float coloumWidth = 125.0f
			\return		    bool
		*******************************************************************************/
		static bool DrawInputDouble(const std::string& tag, const char* label, double* value, float step, float step_fast, const char* format, float coloumWidth = 125.0f);
		/*!***************************************************************************
		*   \brief			This function will draw an error pop up message
			\param  _fn	const std::string& tag, const char* label, char* buffer, size_t bufferSize
			\return		void
		*******************************************************************************/
		static void DrawErrorMessage(const std::string& tag, std::string fileExtension);
	};


}