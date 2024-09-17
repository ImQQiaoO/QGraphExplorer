#include "VertexItem.h"
#include "EdgeItem.h"

VertexItem::VertexItem(const QString &name, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), vertexName(name) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);  // 允许监听几何变化

    // 设置顶点的大小
    setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);
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
    return QGraphicsEllipseItem::itemChange(change, value);
}

// 捕获鼠标点击事件并输出日志
void VertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Vertex" << vertexName << "was clicked!";
    QGraphicsEllipseItem::mousePressEvent(event);  // 调用父类方法继续处理默认行为
}
