#ifndef VERTEXWITHINFO_H
#define VERTEXWITHINFO_H
#include "VertexItem.h"

class VertexWithInfo : public VertexItem {
    QPointF force_;
public:
    VertexWithInfo(QString name, ShapeType shape, QGraphicsItem *parent = nullptr);
    void setForce(QPointF);
    QPointF getForce() const;
};

#endif // VERTEXWITHINFO_H  
