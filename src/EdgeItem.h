#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsLineItem>

class VertexItem;

class EdgeItem : public QGraphicsLineItem {
public:
    EdgeItem(VertexItem *sourceVertex, VertexItem *destVertex, QGraphicsItem *parent = nullptr);

    static QPointF getIntersectionPoint(const QPainterPath &shape, const QLineF &line, bool isSource);
    // 更新边的位置
    void updatePosition();
    VertexItem *get_source() const;
    VertexItem *get_dest() const;

private:
    VertexItem *source;
    VertexItem *dest;
};

#endif // EDGEITEM_H
