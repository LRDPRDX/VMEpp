#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ProgrammableWindow
{
    private :
        json fConfig;
};
