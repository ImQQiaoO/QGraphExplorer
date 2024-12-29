#ifndef ADDITEMS_H
#define ADDITEMS_H
#include "VertexItem.h"
#include "MainWindow.h"
#include <random>
#include <spdlog/spdlog.h>

#include "utils/Locale.hpp"

class AddItems {
    static void addDebug(GraphWidget *graphWidget) {



        // 添加一些顶点
        // 使用梅森旋转生成随机位置，范围-200到200
        // 初始化随机数生成器，使用随机设备作为种子
        std::random_device rd;
        std::mt19937 gen(rd());  // mt19937 是一个随机数生成器

        // 定义一个均匀分布，范围从 -200.0 到 200.0
        std::uniform_real_distribution<double> distrib(-200.0, 200.0);

        graphWidget->addVertex("你好", ShapeType::Circle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("B", ShapeType::Triangle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("C", ShapeType::Circle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("D", ShapeType::Rectangle,
            QPointF(distrib(gen), distrib(gen)));

        // 添加一些边
        graphWidget->addEdge("你好", "B");
        graphWidget->addEdge("你好", "C");
        graphWidget->addEdge("B", "C");
        graphWidget->addEdge("D", "C");

        //graphWidget->addVertex("A", ShapeType::Circle, QPointF(-100, -100));
        //graphWidget->addVertex("B", ShapeType::Triangle, QPointF(100, -100));
        //graphWidget->addVertex("C", ShapeType::Circle, QPointF(0, 100));
        //graphWidget->addVertex("D", ShapeType::Rectangle, QPointF(150, 100));

        /*std::uniform_int_distribution<int> total(175, 250);
        //std::uniform_int_distribution<int> total(750, 1000);
        int total_gen = total(gen);
        std::string num_info = fmt::format("Total gen is: {}", total_gen);
        std::string gbk_num_info = utils::utf8_to_ansi(num_info);
        spdlog::debug(gbk_num_info);
        std::vector<QString> vertex_vec(total_gen);
        for (int i = 0; i < total_gen; ++i) {
            vertex_vec.push_back(QString::number(i));
            graphWidget->addVertex(QString::number(i), static_cast<ShapeType>(i % 3), 
                QPointF(distrib(gen), distrib(gen)));
        }
        for (int i = 0; i < total_gen - 1; ++i) {
            graphWidget->addEdge(QString::number(i), QString::number(i + 1));
        }
        // 随机链接一些顶点
        for (int i = 0; i < total_gen - 1; ++i) {
            // 每个节点连接其他的数量
            std::uniform_int_distribution<int> conj(2, total_gen / 10);
            for (int j = i + 1; j < conj(gen); ++j) {
                std::uniform_int_distribution<int> next(0, total_gen - 1);
                graphWidget->addEdge(QString::number(i), QString::number(next(gen)));
            }



        }*/
    }



public:
    AddItems() = default;

    explicit AddItems(GraphWidget *graphWidget) {
        spdlog::info("AddItems(GraphWidget * graphWidget) Called.");
        addDebug(graphWidget);
    }

};

#endif // ADDITEMS_H