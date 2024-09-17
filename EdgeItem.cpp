#include "EdgeItem.h"
#include "VertexItem.h"
#include <QtMath>

EdgeItem::EdgeItem(VertexItem *sourceVertex, VertexItem *destVertex, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), source(sourceVertex), dest(destVertex) {
    setPen(QPen(Qt::black, 2));
    updatePosition();
}

void EdgeItem::updatePosition() {
    // 获取顶点的中心位置，假设顶点的半径是15
    QPointF sourcePos = source->pos() + QPointF(VERTEX_DIAMETER / 2, VERTEX_DIAMETER / 2);  
    QPointF destPos = dest->pos() + QPointF(VERTEX_DIAMETER / 2, VERTEX_DIAMETER / 2);

    // 计算向量
    QLineF line(sourcePos, destPos);

    // 计算从中心到边缘的偏移量
    qreal radius = VERTEX_DIAMETER / 2;  // 顶点的半径
    if (line.length() > 2 * radius) {
        // 计算单位向量
        QPointF offsetSource = radius * QPointF(line.dx(), line.dy()) / line.length();
        QPointF offsetDest = radius * QPointF(-line.dx(), -line.dy()) / line.length();

        // 调整线条的起点和终点，使它们位于边缘
        line.setP1(line.p1() + offsetSource);
        line.setP2(line.p2() + offsetDest);
    }

    // 设置更新后的线
    setLine(line);
}
