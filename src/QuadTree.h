#pragma once

#include "VertexWithInfo.h"

class QuadTree {
    static constexpr int maxPointsPerNode = 4;

public:
    struct QuadNode;
    QuadNode *root;
    struct QuadNode {
        QRectF boundary;        // 节点的边界
        QPointF centerOfMass;   // 子区域的质心
        int mass;               // 子区域包含的节点数量
        std::vector<VertexWithInfo *> points; // 存储的节点
        QuadNode *children[4];  // 四个子区域

        QuadNode(const QRectF &rect);
        ~QuadNode();

        // 判断是否是叶子节点
        bool isLeaf() const;

        // 划分为四个子节点
        void subdivide();
    };
    QuadTree(const QRectF &boundary);

    ~QuadTree();

    void insert(VertexWithInfo *node);

    static void computeMassAndCenter(QuadNode *node);

    void computeMassAndCenter() const;

private:
    static void insert(QuadNode *node, VertexWithInfo *point);
};
