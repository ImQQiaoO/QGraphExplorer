#include "EdgeItem.h"
#include "VertexItem.h"

EdgeItem::EdgeItem(VertexItem *sourceVertex, VertexItem *destVertex, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), source(sourceVertex), dest(destVertex) {
    // 设置线条样式
    setPen(QPen(Qt::black, 2));

    // 初始时更新位置
    updatePosition();
}

void EdgeItem::updatePosition() {
    // 更新线的位置：从 source 的边缘到 dest 的边缘
    QLineF newLine(mapFromItem(source, 15, 15), mapFromItem(dest, 15, 15));
    setLine(newLine);
}
