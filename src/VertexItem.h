#ifndef VERTEXITEM_H
#define VERTEXITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsScene>

constexpr double VERTEX_DIAMETER = 30.0;

enum class ShapeType {
    Circle,
    Rectangle,
    Triangle
};

class EdgeItem;

class VertexItem : public QGraphicsItem {
public:
    VertexItem(QString name, ShapeType shape, QGraphicsItem *parent = nullptr);
    virtual ~VertexItem() {}
    // 设置填充颜色
    void setBrush(const QBrush &brush);

    // 添加与该顶点相连的边
    void addEdge(EdgeItem *edge);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;    // 重载返回物理形状区域
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setRect(const QRectF &newRect);  // 设置新的矩形
    void setRect(qreal x, qreal y, qreal width, qreal height);

protected:
    // 监听顶点的位置变化
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // 捕获鼠标点击事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

//private:
    QString vertexName;
    ShapeType shapeType;
    QRectF rect;       // 顶点的边界矩形
    QBrush brush;      // 顶点的填充颜色
    QList<EdgeItem *> edges;  // 存储与该顶点相连的边
};

#endif // VERTEXITEM_H
