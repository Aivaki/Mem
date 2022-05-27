#pragma once

#include "memory.hpp"
#include "updater.hpp"


/* CS:GO memory class */ 


#pragma region csgoMemory
#ifndef csgoMemoryDefine
#define csgoMemoryDefine

namespace csgoMemory
{
    class CSGOMemory : public memory::SafeMemory
    {
    public:
        CSGOMemory(const std::wstring_view process_name, const memory::SafeMemory_Access processFlags, const ConstructProcessName tag) noexcept(false) 
            : memory::SafeMemory(process_name, processFlags, tag)
        {
            const std::optional<uint32_t> temp_engineBase = GetModuleBaseAddress(L"engine.dll");
            if (!temp_engineBase.has_value())
                throw std::system_error(GetLastError(), std::system_category(), "Engine.dll not found!");

            const std::optional<uint32_t> temp_clientBase = GetModuleBaseAddress(L"client.dll");
            if (!temp_clientBase.has_value())
                throw std::system_error(GetLastError(), std::system_category(), "client.dll not found!");

            this->m_engineBase = temp_engineBase.value();
            this->m_clientBase = temp_clientBase.value();
        }

    public:

        /* Basic functions for the operation of everything */
        uint32_t getEngineBase() const {
            return m_engineBase;
        }
        //
        uint32_t getClientBase() const {
            return m_clientBase;
        }

    private:
        uint32_t m_engineBase = 0;
        uint32_t m_clientBase = 0;
        uint32_t m_glowBase = 0;
    };
}

#endif
#pragma endregion
