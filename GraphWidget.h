#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>

#include "VertexItem.h"

class GraphWidget : public QGraphicsView {
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);

    // 添加一个顶点到图中
    void addVertex(const QString &name, const QPointF &position);

    // 添加一条边到图中
    void addEdge(const QString &vertex1, const QString &vertex2);

protected:
    // 捕获鼠标滚轮事件，用来实现缩放
    void wheelEvent(QWheelEvent *event) override;

    // 重写绘制事件
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QGraphicsScene *scene;
    QMap<QString, VertexItem *> vertices;  // 用来存储顶点

    // 缩放因子
    qreal scaleFactor = 1.15;

    // 返回两个顶点的中心点
    QPointF getVertexCenter(const QString &vertexName);
};

#endif // GRAPHWIDGET_H
