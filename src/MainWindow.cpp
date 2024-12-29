#include "MainWindow.h"
#include "AddItems.h"
#include "JSONProcessor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    JSONProcessor::json_loader();
    // 创建 GraphWidget，并设置为中央部件
    graphWidget = new GraphWidget(this);
    setCentralWidget(graphWidget);
    // 通过unique_ptr添加节点和边对象
    add_items_ = std::make_unique<AddItems>(graphWidget);

    // 设置窗口大小
    resize(800, 600);  // 设置初始窗口大小为 800x600

}

MainWindow::~MainWindow() = default;