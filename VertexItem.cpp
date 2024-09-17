#include "VertexItem.h"
#include "EdgeItem.h"
#include <QGraphicsScene>

VertexItem::VertexItem(const QString &name, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), vertexName(name) {
    // 设置为可拖动
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);  // 允许监听几何变化
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

