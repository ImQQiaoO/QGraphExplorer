#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // 创建 GraphWidget，并设置为中央部件
    graphWidget = new GraphWidget(this);
    setCentralWidget(graphWidget);

    // 添加一些顶点
    graphWidget->addVertex("A", QPointF(-100, -100));
    graphWidget->addVertex("B", QPointF(100, -100));
    graphWidget->addVertex("C", QPointF(0, 100));
    graphWidget->addVertex("D", QPointF(150, 100));

    // 添加一些边
    graphWidget->addEdge("A", "B");
    graphWidget->addEdge("A", "C");
    graphWidget->addEdge("B", "C");
    graphWidget->addEdge("D", "C");
}

MainWindow::~MainWindow() {
}
