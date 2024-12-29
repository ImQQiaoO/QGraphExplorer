#include "GraphWidget.h"
#include <QWheelEvent>
#include <QTimer>
#include "EdgeItem.h"
#include "QuadTree.h"
#include "VertexWithInfo.h"

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent) {
    // 初始化场景
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿
    // 设置场景的边界
    scene->setSceneRect(-4000, -4000, 8000, 8000);
    //scene->setSceneRect(-800, -800, 800, 800);

    // 使用定时器定时执行力导向布局算法
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {
        // 每次定时器触发时，计算节点间的力并更新位置
        calculateForces(this);
        auto nodes = this->getVertices();
        // 更新节点位置（使用适当的时间步长）
        for (const auto node : nodes) {
            if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
                double timeStep = 0.1;
                QPointF newPos = vertex->pos() + vertex->getForce() * timeStep;
                // 可选：防止节点移动超出可视区域
                QRectF bounds = scene->sceneRect();
                newPos.setX(std::min(bounds.right(), std::max(bounds.left(), newPos.x())));
                newPos.setY(std::min(bounds.bottom(), std::max(bounds.top(), newPos.y())));
                vertex->setPos(newPos); // 更新节点的位置
            }
        }
        scene->update(); // 更新场景显示，确保在UI中看到变化
    });

    // 启动定时器，每 16 毫秒更新一次，相当于每秒约 60 帧
    //timer->start(16);
    timer->start(32);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    // 设置为橡皮筋拖拽模式，支持鼠标框选多个顶点
    setDragMode(QGraphicsView::RubberBandDrag);
}

void GraphWidget::addVertex(const QString &name, const ShapeType shape, const QPointF &position) {
    // 创建自定义顶点项，传入指定的形状
    VertexItem *vertex = new VertexWithInfo(name, shape);

    // 根据不同形状调整大小或其他设置
    switch (shape) {
        case ShapeType::Circle:
            vertex->setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);  // 设置圆形大小
            break;
        case ShapeType::Rectangle:
            vertex->setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);  // 设置矩形大小
            break;
        case ShapeType::Triangle:
            vertex->setRect(0, 0, VERTEX_DIAMETER, VERTEX_DIAMETER);
            break;
    }

    // 设置顶点的颜色或其他样式
    vertex->setBrush(QBrush(Qt::darkGray));

    // 设置顶点的位置
    vertex->setPos(position);
    scene->addItem(vertex);

    // 将顶点存储起来
    vertices[name] = vertex;
}

void GraphWidget::addEdge(const QString &vertex1, const QString &vertex2) {
    // 获取两个顶点
    VertexItem *v1 = vertices[vertex1];
    VertexItem *v2 = vertices[vertex2];

    edges.emplace_back(v1, v2);

    // 创建自定义的边项，并将边加到场景中
    EdgeItem *edge = new EdgeItem(v1, v2);

    scene->addItem(edge);

    // 将边添加到两个顶点中，确保在顶点移动时更新边
    v1->addEdge(edge);
    v2->addEdge(edge);
}

QPointF GraphWidget::getVertexCenter(const QString &vertexName) {
    // 获取顶点的中心点坐标
    if (vertices.contains(vertexName)) {
        QGraphicsItem *vertex = vertices[vertexName];
        return vertex->sceneBoundingRect().center();  // 获取圆的中心
    }
    return {};
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect) {
    // 可以在这里自定义背景绘制
    QGraphicsView::drawBackground(painter, rect);
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    // 检查 Ctrl 键是否按下
    if (event->modifiers() & Qt::ControlModifier) {
        // 检查滚轮滚动的方向
        if (event->angleDelta().y() > 0) {
            // 放大
            scale(1.15, 1.15);
        } else {
            // 缩小
            scale(1.0 / 1.15, 1.0 / 1.15);
        }
    } else {
        // 如果没有按住 Ctrl 键，则调用父类的事件处理程序
        QGraphicsView::wheelEvent(event);
    }
}

QMap<QString, VertexItem *> GraphWidget::getVertices() {
    return vertices;
}

std::vector<std::pair<VertexItem *, VertexItem * >> GraphWidget::getEdges() {
    return edges;
}

static void computeRepulsiveForce(VertexWithInfo *node, QuadTree::QuadNode *quadNode) {
    if (!quadNode) return;

    QPointF delta = node->pos() - quadNode->centerOfMass;
    double distance = std::max(std::hypot(delta.x(), delta.y()), 1.0);

    if (quadNode->isLeaf() || quadNode->boundary.width() / distance < 0.5) {
        // 近似计算排斥力
        double repulsiveForce = 10000.0 * quadNode->mass / (distance * distance);
        QPointF direction = delta / distance;
        node->setForce(node->getForce() + repulsiveForce * direction);
    } else {
        // 递归遍历子节点
        for (auto& i : quadNode->children) {
            computeRepulsiveForce(node, i);
        }
    }
}


void calculateForces(GraphWidget *graphWidget) {
    auto nodes = graphWidget->getVertices();
    auto edges = graphWidget->getEdges();

    // 初始化所有节点的力为零
    for (auto &node : nodes) {
        if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
            vertex->setForce(QPointF(0, 0)); // 初始化力为零
        }
    }

    // 创建四叉树并插入所有节点
    QRectF boundary = graphWidget->scene->sceneRect();
    QuadTree quadTree(boundary);
    for (auto &node : nodes) {
        if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
            quadTree.insert(vertex);
        }
    }
    quadTree.computeMassAndCenter();

    // 使用四叉树计算排斥力
    for (auto &node : nodes) {
        VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node);
        if (vertex) {
            computeRepulsiveForce(vertex, quadTree.root);
        }
    }

    // 计算吸引力（保持不变）
    for (auto &edge : edges) {
        VertexWithInfo *vi = dynamic_cast<VertexWithInfo *>(edge.first);
        VertexWithInfo *vj = dynamic_cast<VertexWithInfo *>(edge.second);
        if (vi && vj) {
            QPointF delta = vi->pos() - vj->pos();
            double distance = std::max(std::hypot(delta.x(), delta.y()), 1.0);
            double attractiveForce = (distance * distance) / 1000.0;
            QPointF direction = delta / distance;
            vi->setForce(vi->getForce() - attractiveForce * direction);
            vj->setForce(vj->getForce() + attractiveForce * direction);
        }
    }
}