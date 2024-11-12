#ifndef ADDITEMS_H
#define ADDITEMS_H
#include <iostream> // TODO: For Debug
#include "VertexItem.h"
#include "MainWindow.h"
#include <random>
#include <spdlog/spdlog.h>

class AddItems {
    static void addDebug(GraphWidget *graphWidget) {
        // 添加一些顶点
        // 使用梅森旋转生成随机位置，范围-200到200
         // 初始化随机数生成器，使用随机设备作为种子
        std::random_device rd;
        std::mt19937 gen(rd());  // mt19937 是一个随机数生成器

        // 定义一个均匀分布，范围从 -200.0 到 200.0
        std::uniform_real_distribution<double> distrib(-200.0, 200.0);

        graphWidget->addVertex("A", ShapeType::Circle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("B", ShapeType::Triangle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("C", ShapeType::Circle,
            QPointF(distrib(gen), distrib(gen)));
        graphWidget->addVertex("D", ShapeType::Rectangle,
            QPointF(distrib(gen), distrib(gen)));

        //graphWidget->addVertex("A", ShapeType::Circle, QPointF(-100, -100));
        //graphWidget->addVertex("B", ShapeType::Triangle, QPointF(100, -100));
        //graphWidget->addVertex("C", ShapeType::Circle, QPointF(0, 100));
        //graphWidget->addVertex("D", ShapeType::Rectangle, QPointF(150, 100));

        // 添加一些边
        graphWidget->addEdge("A", "B");
        graphWidget->addEdge("A", "C");
        graphWidget->addEdge("B", "C");
        graphWidget->addEdge("D", "C");
    }



public:
    AddItems() = default;

    explicit AddItems(GraphWidget *graphWidget) {
        spdlog::info("AddItems(GraphWidget * graphWidget) Called.");
        addDebug(graphWidget);
    }

};

#endif // ADDITEMS_H