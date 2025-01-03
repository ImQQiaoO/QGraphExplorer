#ifndef MOVIE_H
#define MOVIE_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class Movie {
public:
    std::string movieName;
    std::vector<std::string> actorsName;

    friend void from_json(const nlohmann::json &j, Movie &movie);
    friend std::ostream &operator<<(std::ostream &os, const Movie &movie);
};



#endif //MOVIE_H
