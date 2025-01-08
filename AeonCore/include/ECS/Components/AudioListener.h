#pragma once
/*!***************************************************************************
    \file			AudioListener.h
    \author			Wang Yi Long Roy
    \par			w.yilongroy\@digipen.edu
    \date			Nov 19, 2023
    \brief			This header file consists of function declarations for
                    the audio listener component functions for the imgui.


    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "IComponent.h"
#include "fmod.hpp" // Include the FMOD header

namespace AeonCore
{
    class AEON_API AudioListenerComponent : public IComponent
    {
    public:

        AudioListenerComponent();
        //~AudioListenerComponent();

        const FMOD_VECTOR& GetPosition() const { return mPosition; }
        void SetPosition(const FMOD_VECTOR& position);

        const FMOD_VECTOR& GetVelocity() const { return mVelocity; }
        void SetVelocity(const FMOD_VECTOR& velocity);

        const FMOD_VECTOR& GetForward() const { return mForward; }
        void SetForward(const FMOD_VECTOR& forward);

        const FMOD_VECTOR& GetUp() const { return mUp; }
        void SetUp(const FMOD_VECTOR& up);

        void UpdateListenerAttributes();

    private:
        RTTR_ENABLE(IComponent);

        FMOD_VECTOR mPosition;
        FMOD_VECTOR mVelocity;
        FMOD_VECTOR mForward;
        FMOD_VECTOR mUp;
    };
}