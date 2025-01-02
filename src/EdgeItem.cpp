#include "EdgeItem.h"
#include "VertexItem.h"
#include <QPen>

EdgeItem::EdgeItem(VertexItem *sourceVertex, VertexItem *destVertex, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), source(sourceVertex), dest(destVertex) {
    setPen(QPen(Qt::black, 2));
    updatePosition();
}

QPointF EdgeItem::getIntersectionPoint(const QPainterPath &shape, const QLineF &line, bool isSource) {
    QPolygonF polygon = shape.toFillPolygon();
    QLineF adjustedLine = isSource ? line : QLineF(line.p2(), line.p1());
    QPointF intersectPoint;
    bool foundIntersection = false;

    for (int i = 0; i < polygon.size(); ++i) {
        QLineF edge(polygon.at(i), polygon.at((i + 1) % polygon.size()));
        QPointF intersection;
        if (adjustedLine.intersects(edge, &intersection) == QLineF::BoundedIntersection) {
            intersectPoint = intersection;
            foundIntersection = true;
            break;
        }
    }

    // 如果找不到交点，返回原点作为后备（以防止绘制错误）
    if (!foundIntersection) {
        intersectPoint = isSource ? line.p1() : line.p2();
    }

    return intersectPoint;
}

void EdgeItem::updatePosition() {
    if (!source || !dest) return;

    // 获取源和目标的中心位置
    QPointF sourceCenter = source->mapToScene(source->boundingRect().center());
    QPointF destCenter = dest->mapToScene(dest->boundingRect().center());

    // 创建线条，从源节点中心到目标节点中心
    QLineF line(sourceCenter, destCenter);

    // 获取源和目标节点的形状
    QPainterPath sourceShape = source->shape();
    QPainterPath destShape = dest->shape();

    // 将形状从局部坐标转换为场景坐标，并转换为QPainterPath
    QPolygonF sourcePolygonInScene = source->mapToScene(sourceShape.toFillPolygon());
    QPolygonF destPolygonInScene = dest->mapToScene(destShape.toFillPolygon());

    // 将 QPolygonF 转换回 QPainterPath
    QPainterPath sourceShapeInScene;
    sourceShapeInScene.addPolygon(sourcePolygonInScene);

    QPainterPath destShapeInScene;
    destShapeInScene.addPolygon(destPolygonInScene);

    // 计算交点
    QPointF sourceIntersection = getIntersectionPoint(sourceShapeInScene, line, true);
    QPointF destIntersection = getIntersectionPoint(destShapeInScene, line, false);

    // 更新线条的起点和终点为交点
    setLine(QLineF(sourceIntersection, destIntersection));
}

VertexItem *EdgeItem::get_source() const {
    return source;
}

VertexItem *EdgeItem::get_dest() const {
    return dest;
}
