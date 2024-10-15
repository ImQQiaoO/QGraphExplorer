#include "VertexWithInfo.h"

#include <utility>

VertexWithInfo::VertexWithInfo(QString name, ShapeType shape, QGraphicsItem *parent) :
    VertexItem(std::move(name), shape, parent), force_(QPointF(0, 0)) {

}

void VertexWithInfo::setForce(QPointF force) {
    force_ = force;
}

QPointF VertexWithInfo::getForce() const { return force_; }