#pragma once
#include <string>

namespace Engine
{
    namespace Log
    {
        void info(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);
    }
}
