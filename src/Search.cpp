#include "Search.h"

#include "MainWindow.h"

QString Search::target_string;

Search::Search(GraphWidget *graphWidget) {
    lineEdit = new QLineEdit(graphWidget);
    lineEdit->setPlaceholderText("请输入电影或演员名...");
    lineEdit->setGeometry(20, 20, 200, 30);
    connect(lineEdit, &QLineEdit::returnPressed, this, [this, graphWidget]() { return_pressed(graphWidget); });
}


void Search::return_pressed(GraphWidget *graphWidget) const {
    // 获取用户输入的文本
    Search::target_string = lineEdit->text();
    QString text = lineEdit->text();

    spdlog::info(utils::utf8_to_ansi(text.toStdString()));

    QMap<QString, VertexItem *> vertices = graphWidget->getVertices();
    if (vertices.contains(text)) graphWidget->setBrushByName(text);
    else QMessageBox::critical(nullptr, "warning", "未找到相关节点");
}
