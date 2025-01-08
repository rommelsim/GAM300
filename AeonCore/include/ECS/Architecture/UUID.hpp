#pragma once
/*!***************************************************************************
    \file			UUID.hpp
    \author			Chua Yip Xuan
    \par			yipxuan.chua\@digipen.edu
    \brief			This header file consists of a 
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <random>
#include <sstream>

namespace AeonCore
{
    class UUID
    {
    public:
        UUID() : mUUID(GenerateRandomUUID())    {}

        UUID(std::uint64_t _uuid) : mUUID(_uuid)  {}

        bool operator==(const UUID &_rhs) const
        {
            return mUUID == _rhs.mUUID;
        }

        bool operator!=(const UUID &_rhs) const
        {
            return !(*this == _rhs);
        }

        std::string ToString() const
        {
            char buffer[17];
            snprintf(buffer, sizeof(buffer), "%016llx", mUUID);
            return buffer;
        }

        bool FromString(const std::string &_uuidStr)
        {
            std::istringstream iss(_uuidStr);

            if ( iss >> std::hex >> mUUID )
            {
                return true;
            }

            return false;
        }

        std::uint64_t GetUUID() const
        {
            return mUUID;
        }

    private:
        std::uint64_t GenerateRandomUUID()
        {
            //std::lock_guard<std::mutex> lock(mutex);
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<std::uint64_t> dis(0, UINT64_MAX);

            return dis(gen);
        }

        //mutable std::mutex mutex; // Added mutex for synchronization :> oops are u gonna have a deadlock here?
        std::uint64_t mUUID;
    };
}