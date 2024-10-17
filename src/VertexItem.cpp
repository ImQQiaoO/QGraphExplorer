#include "VertexItem.h"
#include "EdgeItem.h"
#include <QPainter>


VertexItem::VertexItem(QString name, ShapeType shape, QGraphicsItem *parent)
    : QGraphicsItem(parent), vertexName(std::move(name)), shapeType(shape) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    // 手动设置顶点的初始大小为一个50x50的矩形
    setRect(QRectF(0, 0, 50, 50));  // 使用自定义的setRect方法
}


// 返回不同形状的QPainterPath，用于点击检测
QPainterPath VertexItem::shape() const {
    QPainterPath path;

    switch (shapeType) {
        case ShapeType::Circle:
            path.addEllipse(boundingRect());
            break;
        case ShapeType::Rectangle:
            path.addRect(boundingRect());
            break;
        case ShapeType::Triangle:
            QPolygonF triangle;
            triangle << QPointF(rect.center().x(), rect.top())
                << QPointF(rect.bottomRight())
                << QPointF(rect.bottomLeft());
            path.addPolygon(triangle);
            break;
    }

    return path;
}


// 设置顶点的填充颜色
void VertexItem::setBrush(const QBrush &newBrush) {
    brush = newBrush;
    update();
}

//QRectF VertexItem::boundingRect() const {
//    return QRectF(0, 0, 50, 50);  // 设置一个合适的边界框
//}
QRectF VertexItem::boundingRect() const {
    return rect;  // 返回当前设置的边界矩形
}

void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::darkGray);

    switch (shapeType) {
        case ShapeType::Circle:
            painter->drawEllipse(boundingRect());
            break;
        case ShapeType::Rectangle:
            painter->drawRect(boundingRect());
            break;
        case ShapeType::Triangle:
            QPolygonF triangle;
            triangle << QPointF(rect.center().x(), rect.top())
                << QPointF(rect.bottomRight())
                << QPointF(rect.bottomLeft());
            painter->drawPolygon(triangle);
            break;
    }
}

void VertexItem::setRect(const QRectF &newRect) {
    rect = newRect;
    update();  // 通知Qt重新绘制
}

void VertexItem::setRect(qreal x, qreal y, qreal width, qreal height) {
    setRect(QRectF(x, y, width, height));  // 调用QRectF版本的setRect
}

void VertexItem::addEdge(EdgeItem *edge) {
    edges.append(edge);
}

// 监听位置变化，实时更新连接的边
QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        // 顶点移动时，更新所有相连的边
        for (EdgeItem *edge : edges) {
            edge->updatePosition();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

// 捕获鼠标点击事件并输出日志
void VertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Vertex" << vertexName << "was clicked!";
    QGraphicsItem::mousePressEvent(event);  // 调用父类方法继续处理默认行为
}

