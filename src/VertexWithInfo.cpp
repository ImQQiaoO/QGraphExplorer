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
