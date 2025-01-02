#ifndef VERTEXITEM_H
#define VERTEXITEM_H

#include "EdgeItem.h"
#include "VertexInfoPopup.h"

class GraphWidget;

constexpr double VERTEX_DIAMETER = 30.0;

enum class ShapeType {
    Circle,
    Rectangle,
    Triangle
};


class VertexItem : public QGraphicsItem {
public:
    VertexItem(QString name, ShapeType shape, QGraphicsItem *parent = nullptr);
    ~VertexItem() override;
    // 设置填充颜色
    void setBrush(const QBrush &brush);

    QBrush getBrush() const;

    // 添加与该顶点相连的边
    void addEdge(EdgeItem *edge);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;    // 重载返回物理形状区域
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setRect(const QRectF &newRect);  // 设置新的矩形
    void setRect(qreal x, qreal y, qreal width, qreal height);

    bool get_visibility() const;
    static void hide_vertex(VertexItem *vertex, GraphWidget *graphWidget, bool hideEdges = true);
    static void show_vertex(VertexItem *vertex, GraphWidget *graphWidget, bool showEdges = true);

protected:
    // 监听顶点的位置变化
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // 捕获鼠标点击事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    //private:
    QString vertexName;
    ShapeType shapeType;
    QRectF rect;       // 顶点的边界矩形
    QBrush brush;      // 顶点的填充颜色
    QList<EdgeItem *> edges;  // 存储与该顶点相连的边
private:
    void hidePopup();
    VertexInfoPopup *popup;
    bool visible;
};

#endif // VERTEXITEM_H
