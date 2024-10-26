#include "VertexWithInfo.h"

VertexWithInfo::VertexWithInfo(QString name, const ShapeType shape, QGraphicsItem *parent) :
    VertexItem(std::move(name), shape, parent), force_(QPointF(0, 0)) {

}

void VertexWithInfo::setForce(const QPointF force) {
    force_ = force;
}

QPointF VertexWithInfo::getForce() const { return force_; }