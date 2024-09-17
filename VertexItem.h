#ifndef VERTEXITEM_H
#define VERTEXITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMap>
#include <QGraphicsScene>

class EdgeItem;

constexpr double VERTEX_DIAMETER = 30.0;

class VertexItem : public QGraphicsEllipseItem {
public:
    VertexItem(const QString &name, QGraphicsItem *parent = nullptr);

    // 添加与该顶点相连的边
    void addEdge(EdgeItem *edge);

protected:
    // 监听顶点的位置变化
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QString vertexName;
    QList<EdgeItem *> edges;  // 存储与该顶点相连的边
};

#endif // VERTEXITEM_H
