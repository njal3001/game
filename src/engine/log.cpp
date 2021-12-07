#include "engine/log.h"
#include <iostream>

namespace Engine
{
    namespace 
    {
        void print(const std::string& title, const std::string& message)
        {
            std::cout << title << ": " << message << '\n';
        }
    }

    void Log::info(const std::string& message)
    {
        print("INFO", message);
    }

    void Log::warn(const std::string& message)
    {
        print("WARN", message);
    }

    void Log::error(const std::string& message)
    {
        print("ERROR", message);
    }
}
