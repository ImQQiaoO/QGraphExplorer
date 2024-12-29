#include "JSONProcessor.h"
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include "Movie.h" 

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

    std::vector<Movie> movies;
    for (size_t i = 0; i < 50; ++i) {
        Movie movie = j[i].get<Movie>();
        movies.push_back(movie);
    }

    for (const auto &movie : movies) {
        std::cout << movie << '\n';
    }
}
