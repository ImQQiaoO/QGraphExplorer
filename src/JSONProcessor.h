#pragma once
#include <nlohmann/json.hpp>

#include "Movie.h"


class JSONProcessor {
    using json = nlohmann::json;
public:
    static std::vector<Movie> movies;
    static void json_loader();  // load json file to a static list
};
