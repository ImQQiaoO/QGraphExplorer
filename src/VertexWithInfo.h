#ifndef VERTEXWITHINFO_H
#define VERTEXWITHINFO_H
#include "VertexItem.h"
#include <QPointF>
#include <mutex>


class VertexWithInfo : public VertexItem {
public:
    VertexWithInfo(QString name, ShapeType shape, QGraphicsItem *parent = nullptr);

    QPointF getForce() const;

    void setForce(const QPointF &f);

    void addForce(const QPointF &f) {
        std::lock_guard<std::mutex> lock(mutex);
        force += f;
    }

    // 获取和设置速度
    QPointF getVelocity() const;
    void setVelocity(const QPointF &velocity);
    // 应用力来更新位置和速度
    void applyForce(const QRectF &bounds);

private:
    QString name;
    QPointF force;
    QPointF velocity;
    mutable std::mutex mutex; // 保护force的互斥量
};


#endif // VERTEXWITHINFO_H  
