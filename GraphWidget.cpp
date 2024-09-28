#include "GraphWidget.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QWheelEvent>
#include "VertexItem.h"
#include "EdgeItem.h"

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent) {
    // 初始化场景
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿

    // 设置场景的边界
    scene->setSceneRect(-400, -400, 800, 800);

    // 设置为橡皮筋拖拽模式，支持鼠标框选多个顶点
    setDragMode(QGraphicsView::RubberBandDrag);
}

void GraphWidget::addVertex(const QString &name, ShapeType shape, const QPointF &position) {
    // 创建自定义顶点项，传入指定的形状
    VertexItem *vertex = new VertexItem(name, shape);

    // 根据不同形状调整大小或其他设置
    switch (shape) {
    case ShapeType::Circle:
            vertex->setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);  // 设置圆形大小
            break;
        case ShapeType::Rectangle:
            vertex->setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);  // 设置矩形大小
            break;
        case ShapeType::Triangle:
            // 对于三角形，不使用 `setRect`，直接通过 `paint` 渲染
            break;
    }

    // 设置顶点的颜色或其他样式
    vertex->setBrush(QBrush(Qt::darkGray));

    // 设置顶点的位置
    vertex->setPos(position);
    scene->addItem(vertex);

    // 将顶点存储起来
    vertices[name] = vertex;
}

void GraphWidget::addEdge(const QString &vertex1, const QString &vertex2) {
    // 获取两个顶点
    VertexItem *v1 = vertices[vertex1];
    VertexItem *v2 = vertices[vertex2];

    // 创建自定义的边项，并将边加到场景中
    EdgeItem *edge = new EdgeItem(v1, v2);

    scene->addItem(edge);

    // 将边添加到两个顶点中，确保在顶点移动时更新边
    v1->addEdge(edge);
    v2->addEdge(edge);
}

QPointF GraphWidget::getVertexCenter(const QString &vertexName) {
    // 获取顶点的中心点坐标
    if (vertices.contains(vertexName)) {
        QGraphicsItem *vertex = vertices[vertexName];
        return vertex->sceneBoundingRect().center();  // 获取圆的中心
    }
    return QPointF();
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect) {
    // 可以在这里自定义背景绘制
    QGraphicsView::drawBackground(painter, rect);
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    // 检查 Ctrl 键是否按下
    if (event->modifiers() & Qt::ControlModifier) {
        // 检查滚轮滚动的方向
        if (event->angleDelta().y() > 0) {
            // 放大
            scale(1.15, 1.15);
        } else {
            // 缩小
            scale(1.0 / 1.15, 1.0 / 1.15);
        }
    } else {
        // 如果没有按住 Ctrl 键，则调用父类的事件处理程序
        QGraphicsView::wheelEvent(event);
    }
}
