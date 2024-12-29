#include "MainWindow.h"
#include "AddItems.h"
#include <QLineEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // 创建 GraphWidget，并设置为中央部件
    graphWidget = new GraphWidget(this);
    setCentralWidget(graphWidget);

    // 通过unique_ptr添加节点和边对象
    add_items_ = std::make_unique<AddItems>(graphWidget);

    // 创建 QLineEdit 控件作为文本输入框
    lineEdit = new QLineEdit(this);  // 创建一个新的文本输入框
    lineEdit->setPlaceholderText("请输入文本...");  // 设置输入框的占位符文本
    lineEdit->setGeometry(20, 20, 200, 30);  // 设置文本输入框的位置和大小

    connect(lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_returnPressed);

    // 设置窗口大小
    resize(800, 600);  // 设置初始窗口大小为 800x600

}

// 槽函数，捕获回车键事件
void MainWindow::on_returnPressed() {
    // 获取用户输入的文本
    QString text = lineEdit->text();

    // 输出文本到控制台（你可以根据需要做其他处理）
    spdlog::info( utils::utf8_to_ansi(text.toStdString()));

    // 处理用户输入的文本，可以进行其他操作，如提交数据等
}


MainWindow::~MainWindow() = default;