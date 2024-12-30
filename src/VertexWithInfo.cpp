#include "VertexWithInfo.h"

VertexWithInfo::VertexWithInfo(QString name, const ShapeType shape, QGraphicsItem *parent) :
    VertexItem(std::move(name), shape, parent), force(0, 0), velocity(0, 0) {
}



QPointF VertexWithInfo::getForce() const {
    return force;
}

void VertexWithInfo::setForce(const QPointF& f) {
    force = f;
}

QPointF VertexWithInfo::getVelocity() const {
    return velocity;
}

// 设置速度
void VertexWithInfo::setVelocity(const QPointF &v) {
    velocity = v;
}

void VertexWithInfo::applyForce(const QRectF &bounds) {
    double damping = 0.85;
    double timeStep = 0.05;
    QPointF currentForce = getForce();
    QPointF currentVelocity = getVelocity();

    // 更新速度：速度 = 阻尼 * 速度 + 力 * 时间步长
    currentVelocity = currentVelocity * damping + currentForce * timeStep;

    // 更新位置：位置 = 当前位置 + 速度
    QPointF newPos = pos() + currentVelocity;

    // 限制新位置在边界内
    newPos.setX(std::min(bounds.right(), std::max(bounds.left(), newPos.x())));
    newPos.setY(std::min(bounds.bottom(), std::max(bounds.top(), newPos.y())));

    // 应用更新
    setVelocity(currentVelocity);
    setPos(newPos);
}