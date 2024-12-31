#include "JSONProcessor.h"
#include <fstream>
#include <sstream>
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "Movie.h"

std::vector<Movie> JSONProcessor::movies;

namespace {
    std::string json_path = "movie.json";
}


void JSONProcessor::json_loader() {
    std::string rootDir;
#ifdef BUILD_WITH_CMAKE
    rootDir = std::string(ROOT_DIR) + '/';
#endif
    std::ifstream input_file(rootDir + json_path);

    if (!input_file.is_open()) {
        QMessageBox::critical(nullptr, "Error", "Can't read JSON file.");
    }
    // 解析 JSON 数据
    json j;
    input_file >> j;

    for (size_t i = 0; i < 70; ++i) {
        Movie movie = j[i].get<Movie>();
        movies.push_back(movie);
    }

    // For debugging
    for (const auto &movie : movies) {
        std::stringstream ss;
        ss << movie;
        spdlog::info("\n" + ss.str());
    }

}
