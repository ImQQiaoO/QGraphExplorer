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

    QPointF getVelocity() const {
        return velocity;
    }

    void setVelocity(const QPointF &v) {
        velocity = v;
    }

private:
    QString name;
    QPointF force;
    QPointF velocity;
    mutable std::mutex mutex; // 保护force的互斥量
};


#endif // VERTEXWITHINFO_H  
