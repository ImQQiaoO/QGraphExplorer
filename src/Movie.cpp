#include "Movie.h"
#include "utils/Locale.hpp"

using json = nlohmann::json;

void from_json(const json &j, Movie &movie) {
    movie.movieName = j.at("title").get<std::string>();
    for (const auto &actor : j.at("actors")) {
        movie.actorsName.push_back(actor.at("name").get<std::string>());
    }
}


std::ostream &operator<<(std::ostream &os, const Movie &movie) {
    os << "Movie Name: " << utils::utf8_to_ansi(movie.movieName) << '\n';
    os << "Actors: [\n";
    for (size_t i = 0; i < movie.actorsName.size(); ++i) {
        if (i == movie.actorsName.size() - 1)
            os << "  " << utils::utf8_to_ansi(movie.actorsName[i]) << '\n';
        else
            os << "  " << utils::utf8_to_ansi(movie.actorsName[i]) << ",\n";
    }
    os << ']';
    return os;
}