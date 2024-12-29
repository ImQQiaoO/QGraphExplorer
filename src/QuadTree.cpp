#include "QuadTree.h"

QuadTree::QuadNode::QuadNode(const QRectF &rect) :
    boundary(rect), centerOfMass(0, 0), mass(0), children {nullptr, nullptr, nullptr, nullptr} {
}

QuadTree::QuadNode::~QuadNode() {
    for (const auto &i : children) {
        delete i;
    }
}

bool QuadTree::QuadNode::isLeaf() const {
    return children[0] == nullptr;
}

void QuadTree::QuadNode::subdivide() {
    double x = boundary.x();
    double y = boundary.y();
    double w = boundary.width() / 2;
    double h = boundary.height() / 2;

    children[0] = new QuadNode(QRectF(x, y, w, h));            // 左上
    children[1] = new QuadNode(QRectF(x + w, y, w, h));       // 右上
    children[2] = new QuadNode(QRectF(x, y + h, w, h));       // 左下
    children[3] = new QuadNode(QRectF(x + w, y + h, w, h));   // 右下
}

QuadTree::QuadTree(const QRectF &boundary) {
    root = new QuadNode(boundary);
}

QuadTree::~QuadTree() {
    delete root;
}

void QuadTree::insert(VertexWithInfo *node) {
    insert(root, node);
}

void QuadTree::computeMassAndCenter(QuadNode *node) {
    if (!node) return;
    if (node->isLeaf()) {
        for (auto point : node->points) {
            node->centerOfMass += point->pos();
            node->mass++;
        }
        if (node->mass > 0) {
            node->centerOfMass /= node->mass;
        }
    } else {
        for (const auto &i : node->children) {
            computeMassAndCenter(i);
            if (i->mass > 0) {
                node->centerOfMass += i->centerOfMass * i->mass;
                node->mass += i->mass;
            }
        }
        if (node->mass > 0) {
            node->centerOfMass /= node->mass;
        }
    }
}

void QuadTree::computeMassAndCenter() const {
    computeMassAndCenter(root);
}

void QuadTree::insert(QuadNode *node, VertexWithInfo *point) {
    if (!node->boundary.contains(point->pos())) {
        return;
    }

    if (node->isLeaf() && node->points.size() < maxPointsPerNode) {
        node->points.push_back(point);
    } else {
        if (node->isLeaf()) {
            node->subdivide();
            for (const auto p : node->points) {
                for (const auto &i : node->children) {
                    insert(i, p);
                }
            }
            node->points.clear();
        }
        for (auto &i : node->children) {
            insert(i, point);
        }
    }
}



