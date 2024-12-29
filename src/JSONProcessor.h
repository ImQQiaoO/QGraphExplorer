#pragma once
#include <nlohmann/json.hpp>


class JSONProcessor {
    using json = nlohmann::json;

public:
    static void json_loader();  // load json file to a static list
};