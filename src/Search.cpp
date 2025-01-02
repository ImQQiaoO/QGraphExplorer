#include "Search.h"
#include <QToolButton>
#include <QMenu>
#include "GraphWidget.h"
#include <QMessageBox>
#include <set>
#include <spdlog/spdlog.h>

#include "JSONProcessor.h"
#include "utils/Locale.hpp" 

QString Search::target_string;

namespace {
    void show_items_groups(VertexItem *vertex, GraphWidget *graphWidget) {
        if (!vertex) return;
        for (EdgeItem *edge : vertex->get_edges()) {
            if (!edge) continue;
            // 检查 source 和 dest，找到与当前顶点相连的其他顶点
            VertexItem *source = edge->get_source();
            VertexItem *dest = edge->get_dest();
            if (source && source != vertex) {
                //connectedVertices.insert(source);
                VertexItem::show_vertex(source, graphWidget);
                VertexItem::show_vertex(vertex, graphWidget);
            }
            if (dest && dest != vertex) {
                //connectedVertices.insert(dest);
                VertexItem::show_vertex(dest, graphWidget);
                VertexItem::show_vertex(vertex, graphWidget);
            }
        }
    }
}

Search::Search(GraphWidget *graphWidget) : QObject(graphWidget), currentCategory("聚焦") {
    categoryList = {
        "聚焦",
        "相关电影",
        "测试2"
    };

    // 创建 QLineEdit
    lineEdit = new QLineEdit(graphWidget);
    lineEdit->setPlaceholderText("请输入电影或演员名...");
    lineEdit->setGeometry(20, 20, 200, 30);
    connect(lineEdit, &QLineEdit::returnPressed, this, [this, graphWidget]() { enter_pressed(graphWidget); });

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
    connect(dropDownButton, &QToolButton::clicked, this, [this, graphWidget]() { button_pressed(graphWidget); });
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
    Search::target_string = lineEdit->text(); // 获取用户输入的文本
    currentCategory = dropDownButton->text(); // 更新当前类别
    dropDownButton->setText(currentCategory); // 更新按钮显示文本
    if (currentCategory != "还原") {
        befClk = currentCategory;
    }

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
        if (vertices.contains(target_string)) {
            graphWidget->focus_on_vertex(target_string);
        } else {
            QMessageBox::critical(nullptr, "警告", "未找到相关节点");
        }
    } else if (currentCategory == "相关电影") {
        QMap<QString, VertexItem *> vertices = graphWidget->getVertices();
        if (!vertices.contains(target_string)) {
            QMessageBox::critical(nullptr, "警告", "未找到相关节点");
            return;
        }
        currentCategory = "还原";
        dropDownButton->setText("还原");

        graphWidget->focus_on_vertex(target_string);

        std::thread t(Search::search_relevant_movies, graphWidget);
        t.detach();
    } else if (currentCategory == "还原") {
        for (const auto &v : graphWidget->getVertices()) {
            VertexItem::show_vertex(v, graphWidget);
        }
        currentCategory = befClk;
        dropDownButton->setText(befClk);
    }
}

void Search::enter_pressed(GraphWidget *graphWidget) const {
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

void Search::search_relevant_movies(GraphWidget *graphWidget) {
    VertexItem *vertex = nullptr;
    bool is_movie = false;
    for (const auto &movie : JSONProcessor::movies) {
        if (QString::fromStdString(movie.movieName) == Search::target_string) {
            vertex = graphWidget->getVertices()[QString::fromStdString(movie.movieName)];
            is_movie = true;
            break;
        }
    }
    // 先隐藏全部节点
    for (const auto &v : graphWidget->getVertices()) {
        VertexItem::hide_vertex(v, graphWidget);
    }
    std::set<VertexItem *> to_show;
    if (is_movie) {
        to_show.insert(vertex);
    } else {
        for (const auto &movie : JSONProcessor::movies) {
            for (const auto &actor : movie.actorsName) {
                if (QString::fromStdString(actor) == Search::target_string) {
                    vertex = graphWidget->getVertices()[QString::fromStdString(movie.movieName)];
                    to_show.insert(vertex);
                    break;
                }
            }
        }
    }
    for (VertexItem *v : to_show) {
        show_items_groups(v, graphWidget);
    }
}
