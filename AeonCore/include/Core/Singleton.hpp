#pragma once
/*!***************************************************************************
    \file			Singleton.hpp
    \author			Chua Yip Xuan
    \par			yipxuan.chua\@digipen.edu
    \brief			This header file consists of a Singleton class that allows
                    multiple inheritance, and provide an instance of the class.
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
namespace AeonCore
{
    template<typename T>
    class Singleton
    {
    public:
        static T &GetInstance()
        {
            static T instance;
            return instance;
        }

        Singleton(const Singleton &) = delete;
        void operator=(const Singleton &) = delete;

    protected:
        Singleton() = default; // Private constructor to prevent instantiation.
        virtual ~Singleton() = default;
    };
}
