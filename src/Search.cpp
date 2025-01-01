#include "Search.h"
#include <QToolButton>
#include <QMenu>
#include "GraphWidget.h"
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include "utils/Locale.hpp" 

QString Search::target_string;

Search::Search(GraphWidget *graphWidget) : QObject(graphWidget), currentCategory("聚焦") {
    categoryList = {
        "聚焦",
        "测试1",
        "测试2"
    };

    // 创建 QLineEdit
    lineEdit = new QLineEdit(graphWidget);
    lineEdit->setPlaceholderText("请输入电影或演员名...");
    lineEdit->setGeometry(20, 20, 200, 30);
    connect(lineEdit, &QLineEdit::returnPressed, this, [this, graphWidget]() { return_pressed(graphWidget); });

    // 创建下拉按钮
    dropDownButton = new QToolButton(graphWidget);
    dropDownButton->setText("聚焦"); // 初始类别
    dropDownButton->setPopupMode(QToolButton::MenuButtonPopup);
    dropDownButton->setArrowType(Qt::DownArrow);
    dropDownButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    // 创建菜单并添加选项
    QMenu *menu = new QMenu(dropDownButton);
    for (const auto &category : categoryList) {
        menu->addAction(category);
    }
    dropDownButton->setMenu(menu);

    // 设置下拉按钮的位置
    dropDownButton->setGeometry(230, 20, 80, 30);

    connect(menu, &QMenu::triggered, this, &Search::dropdown_option_selected);
    connect(dropDownButton, &QToolButton::clicked, this, [this, graphWidget]() { return_pressed(graphWidget); });
}

void Search::dropdown_option_selected(const QAction *action) {
    if (action) {
        QString selectedOption = action->text();
        currentCategory = selectedOption; // 更新当前类别
        dropDownButton->setText(currentCategory); // 更新按钮显示文本

        std::stringstream ss;
        ss << " 选择的类别:";
        spdlog::info("{}", utils::utf8_to_ansi(ss.str()) + utils::utf8_to_ansi(currentCategory.toStdString()));
    }
}

void Search::button_pressed(GraphWidget *graphWidget) {
    currentCategory = dropDownButton->text(); // 更新当前类别
    dropDownButton->setText(currentCategory); // 更新按钮显示文本

    // DEBUG
    std::stringstream ss;
    ss << " 按下时的类别:";
    spdlog::info("{}", utils::utf8_to_ansi(ss.str()) + utils::utf8_to_ansi(currentCategory.toStdString()));

    if (currentCategory == "聚焦") {
        // 获取用户输入的文本
        Search::target_string = lineEdit->text();
        QString text = lineEdit->text();

        spdlog::info(utils::utf8_to_ansi(text.toStdString()));

        QMap<QString, VertexItem *> vertices = graphWidget->getVertices();
        if (vertices.contains(text)) {
            graphWidget->focus_on_vertex(text);
        } else {
            QMessageBox::critical(nullptr, "警告", "未找到相关节点");
        }

        std::stringstream ss1;
        ss1 << " 按下时的类别:";
        spdlog::info("{}", utils::utf8_to_ansi(ss1.str()) + utils::utf8_to_ansi(currentCategory.toStdString()));
    }
}

void Search::return_pressed(GraphWidget *graphWidget) const {
    // 获取用户输入的文本
    Search::target_string = lineEdit->text();
    QString text = lineEdit->text();

    spdlog::info(utils::utf8_to_ansi(text.toStdString()));

    QMap<QString, VertexItem *> vertices = graphWidget->getVertices();
    if (vertices.contains(text)) {
        graphWidget->focus_on_vertex(text);
    } else {
        QMessageBox::critical(nullptr, "警告", "未找到相关节点");
    }

    std::stringstream ss;
    ss << " 按下时的类别:";
    spdlog::info("{}", utils::utf8_to_ansi(ss.str()) + utils::utf8_to_ansi(currentCategory.toStdString()));
}
