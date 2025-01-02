#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QMap>
#include <vector>

#include "VertexItem.h"

// TODO: 应添加addVertex和addEdge的单参数重载版本
class GraphWidget : public QGraphicsView {
    Q_OBJECT

public:

    explicit GraphWidget(QWidget *parent = nullptr);

    // 添加一个顶点到图中
    void addVertex(const QString &name, ShapeType shape, const QPointF &position);

    void focus_on_vertex(const QString &name);

    // 添加一条边到图中
    void addEdge(const QString &vertex1, const QString &vertex2);

    int get_vertex_size() const;

    QGraphicsScene *scene;
    // 获取顶点
    QMap<QString, VertexItem *> getVertices() const;
    QGraphicsView *view;
    std::vector<std::pair<VertexItem *, VertexItem *>> getEdges() const;
    QRectF getSceneBounds() const;

protected:
    // 捕获鼠标滚轮事件，用来实现缩放
    void wheelEvent(QWheelEvent *event) override;

    // 重写绘制事件
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QMap<QString, VertexItem *> vertices;  // 用来存储顶点
    std::vector<std::pair<VertexItem *, VertexItem * >> edges;    // 用来存储边

    // 缩放因子
    qreal scaleFactor = 1.15;

    int vertex_size = 0;

    // 返回两个顶点的中心点
    QPointF getVertexCenter(const QString &vertexName);
};

void calculateForces(const GraphWidget *graphWidget);

#endif // GRAPHWIDGET_H
