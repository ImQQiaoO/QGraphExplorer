#ifndef ADDITEMS_H
#define ADDITEMS_H
#include <iostream> // TODO: For Debug

#include "VertexItem.h"
#include "MainWindow.h"

class AddItems {

    void add_debug(GraphWidget *graphWidget) {
        // 添加一些顶点
        graphWidget->addVertex("A", ShapeType::Circle, QPointF(-100, -100));
        graphWidget->addVertex("B", ShapeType::Triangle, QPointF(100, -100));
        graphWidget->addVertex("C", ShapeType::Circle, QPointF(0, 100));
        graphWidget->addVertex("D", ShapeType::Rectangle, QPointF(150, 100));

        // 添加一些边
        graphWidget->addEdge("A", "B");
        graphWidget->addEdge("A", "C");
        graphWidget->addEdge("B", "C");
        graphWidget->addEdge("D", "C");
    }

public:
    AddItems() = default;

    AddItems(GraphWidget *graphWidget) {
        std::cout << "AddItems(GraphWidget *graphWidget) Called" << '\n';
        add_debug(graphWidget);
    }
    
};

#endif // ADDITEMS_H