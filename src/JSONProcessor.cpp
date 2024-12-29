#include "JSONProcessor.h"
#include <fstream>
#include <QMessageBox>

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
}
