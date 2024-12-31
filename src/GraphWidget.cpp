#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QWheelEvent>
#include <cmath>
#include <limits>
#include <thread>
#include <vector>
#include <algorithm>
#include "GraphWidget.h"
#include "EdgeItem.h"
#include "VertexWithInfo.h"

#include "AddItems.h"

namespace {
    constexpr double THETA = 0.8;  // 用于判断是否使用质心的阈值

    // 定义一个四叉树类，用于空间分割
    class QuadTree {
    public:
        QRectF boundary;  // 当前区域的边界
        int capacity;     // 当前区域最大容纳的节点数
        bool divided;     // 是否已被分割
        QPointF centerOfMass;  // 质心
        double mass;         // 区域的总质量（节点数量）
        std::vector<VertexWithInfo *> points;  // 存储在此区域的节点
        QuadTree *NW;  // 北西
        QuadTree *NE;  // 北东
        QuadTree *SW;  // 南西
        QuadTree *SE;  // 南东

        // 构造函数
        QuadTree(const QRectF& boundary, int capacity)
            : boundary(boundary), capacity(capacity), divided(false), centerOfMass(0, 0), mass(0),
            NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr) {
        }

        // 插入一个点到四叉树中
        bool insert(VertexWithInfo *vertex) {
            if (!boundary.contains(vertex->pos())) {
                return false;  // 如果点不在当前区域内，则不插入
            }

            if (points.size() < capacity && !divided) {
                points.push_back(vertex);
                mass += 1;
                // 更新质心
                centerOfMass += vertex->pos();
                return true;
            }

            // 如果该区域已满且未分割，则分割区域
            if (!divided) {
                subdivide();
            }

            // 尝试将点插入四个子区域
            if (NW->insert(vertex)) {} else if (NE->insert(vertex)) {} else if (SW->insert(vertex)) {} else if (SE->insert(vertex)) {} else {
                // 插入失败，可能因为边界计算错误
                return false;
            }

            // 更新当前节点的质量和质心
            updateCenterOfMass();

            return true;
        }

        // 分割四叉树
        void subdivide() {
            double midX = (boundary.left() + boundary.right()) / 2;
            double midY = (boundary.top() + boundary.bottom()) / 2;
            QRectF nw(boundary.left(), boundary.top(), boundary.width() / 2, boundary.height() / 2);
            QRectF ne(midX, boundary.top(), boundary.width() / 2, boundary.height() / 2);
            QRectF sw(boundary.left(), midY, boundary.width() / 2, boundary.height() / 2);
            QRectF se(midX, midY, boundary.width() / 2, boundary.height() / 2);

            NW = new QuadTree(nw, capacity);
            NE = new QuadTree(ne, capacity);
            SW = new QuadTree(sw, capacity);
            SE = new QuadTree(se, capacity);

            divided = true;

            // 重新插入当前节点，仅插入到一个子区域
            for (auto &point : points) {
                if (NW->insert(point)) continue;
                if (NE->insert(point)) continue;
                if (SW->insert(point)) continue;
                if (SE->insert(point)) continue;
            }
            points.clear();  // 清空当前节点

            // 更新质心和质量
            updateCenterOfMass();
        }

        // 计算某个节点的斥力
        QPointF calculateForce(VertexWithInfo *vertex) {
            QPointF force(0, 0);
            if (mass == 0 || (mass == 1 && points.empty())) {
                return force;  // 没有质量或者只有自身
            }

            if (!divided) {
                // 叶子节点，逐一计算斥力
                for (auto &other : points) {
                    if (other != vertex) {
                        QPointF delta = vertex->pos() - other->pos();
                        double distance = std::max(std::hypot(delta.x(), delta.y()), 0.01);
                        double repulsiveForce = (300 * mass) / (distance * distance); // 调整后的斥力常数
                        double maxRepulsiveForce = 1000.0; // 斥力上限
                        repulsiveForce = std::min(repulsiveForce, maxRepulsiveForce);
                        QPointF direction = delta / distance;
                        force += repulsiveForce * direction;
                    }
                }
            } else {
                // 内部节点，决定是否使用近似
                QPointF delta = vertex->pos() - centerOfMass;
                double distance = std::max(std::hypot(delta.x(), delta.y()), 0.01);
                if (boundary.width() / distance < THETA) {
                    // 使用质心近似
                    double repulsiveForce = (300 * mass) / (distance * distance); // 调整后的斥力常数
                    double maxRepulsiveForce = 1000.0; // 斥力上限
                    repulsiveForce = std::min(repulsiveForce, maxRepulsiveForce);
                    QPointF direction = delta / distance;
                    force += repulsiveForce * direction;
                } else {
                    // 递归计算子节点的力
                    if (NW) force += NW->calculateForce(vertex);
                    if (NE) force += NE->calculateForce(vertex);
                    if (SW) force += SW->calculateForce(vertex);
                    if (SE) force += SE->calculateForce(vertex);
                }
            }
            return force;
        }

        // 计算整个四叉树的质心和质量
        void updateCenterOfMass() {
            if (!divided) {
                if (points.empty()) {
                    mass = 0;
                    centerOfMass = QPointF(0, 0);
                } else {
                    // 计算当前节点的质心和质量
                    double totalX = 0, totalY = 0;
                    for (auto &point : points) {
                        totalX += point->pos().x();
                        totalY += point->pos().y();
                    }
                    mass = static_cast<double>(points.size());
                    centerOfMass = QPointF(totalX / mass, totalY / mass);
                }
            } else {
                // 汇总子节点的质心和质量
                double totalMass = 0;
                double totalX = 0, totalY = 0;

                if (NW->mass > 0) {
                    totalMass += NW->mass;
                    totalX += NW->centerOfMass.x() * NW->mass;
                    totalY += NW->centerOfMass.y() * NW->mass;
                }
                if (NE->mass > 0) {
                    totalMass += NE->mass;
                    totalX += NE->centerOfMass.x() * NE->mass;
                    totalY += NE->centerOfMass.y() * NE->mass;
                }
                if (SW->mass > 0) {
                    totalMass += SW->mass;
                    totalX += SW->centerOfMass.x() * SW->mass;
                    totalY += SW->centerOfMass.y() * SW->mass;
                }
                if (SE->mass > 0) {
                    totalMass += SE->mass;
                    totalX += SE->centerOfMass.x() * SE->mass;
                    totalY += SE->centerOfMass.y() * SE->mass;
                }

                mass = totalMass;
                if (mass > 0) {
                    centerOfMass = QPointF(totalX / mass, totalY / mass);
                } else {
                    centerOfMass = QPointF(0, 0);
                }
            }
        }
    };

    QRectF calculateDynamicBoundary(const QMap<QString, VertexItem *> &nodes) {
        if (nodes.isEmpty()) {
            return {-1000, -1000, 2000, 2000};
        }

        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double maxY = std::numeric_limits<double>::lowest();

        for (auto node : nodes) {
            if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
                QPointF pos = vertex->pos();
                minX = std::min(minX, pos.x());
                minY = std::min(minY, pos.y());
                maxX = std::max(maxX, pos.x());
                maxY = std::max(maxY, pos.y());
            }
        }

        double padding = 200.0;
        return {minX - padding, minY - padding, (maxX - minX) + 2 * padding, (maxY - minY) + 2 * padding};
    }

} // 匿名命名空间

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent) {
    // 初始化场景
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿
    // 设置场景的边界
    QRectF dynamicBoundary = calculateDynamicBoundary(this->getVertices()); // 根据节点计算边界
    QRectF extendedBoundary = dynamicBoundary.adjusted(-5000, -5000, 5000, 5000); // 增大边界
    scene->setSceneRect(extendedBoundary);
    // 使用定时器定时执行力导向布局算法
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {
        // 每次定时器触发时，计算节点间的力并更新位置

        auto start = std::chrono::high_resolution_clock::now();

        // 每次定时器触发时，计算节点间的力并更新位置
        calculateForces(this);
        // DEBUG
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        qDebug() << "Force Calculation Time:" << elapsed.count() << "seconds";

        auto nodes = this->getVertices();
        // 更新节点位置（使用适当的时间步长和阻尼）

        double damping = 0.85; // 阻尼
        double timeStep = 0.75; // 时间步长
        double maxSpeed = 75.0; // 最大移动速度
        double minMoveThreshold = 0.1; // 最小移动阈值

        for (auto node : nodes) {
            if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
                QPointF force = vertex->getForce();
                QPointF velocity = vertex->getVelocity();

                // 更新速度
                velocity = velocity * damping + force * timeStep;

                // 限制最大速度
                double speed = std::hypot(velocity.x(), velocity.y());
                if (speed > maxSpeed) {
                    velocity = velocity / speed * maxSpeed;
                }

                // 更新位置
                QPointF newPos = vertex->pos() + velocity;

                // 检查是否移动超过最小阈值
                QPointF movement = newPos - vertex->pos();
                if (std::hypot(movement.x(), movement.y()) < minMoveThreshold) {
                    // 如果移动量小于阈值，则停止移动
                    vertex->setVelocity(QPointF(0, 0));
                    continue;
                }

                // 限制节点移动在边界内
                QRectF bounds = scene->sceneRect();
                newPos.setX(std::min(bounds.right(), std::max(bounds.left(), newPos.x())));
                newPos.setY(std::min(bounds.bottom(), std::max(bounds.top(), newPos.y())));

                // 应用更新
                vertex->setVelocity(velocity);
                vertex->setPos(newPos);
            }
        }
        scene->update(); // 更新场景显示，确保在UI中看到变化
    });

    // 启动定时器，每 32 毫秒更新一次，相当于每秒约 30 帧
    timer->start(32);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    // 设置为橡皮筋拖拽模式，支持鼠标框选多个顶点
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    vertex_size = 0;       // DEBUG: 用于调试
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
    ++vertex_size;
}

void GraphWidget::setBrushByName(QString name) {
    vertices[name]->setBrush(QBrush(Qt::red));

    this->centerOn(vertices[name]->sceneBoundingRect().center());


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

int GraphWidget::get_vertex_size() const {
    return vertex_size;
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
    if (event->modifiers() & Qt::ControlModifier) {
        double scale_factor = 1.15;
        if (event->angleDelta().y() < 0)
            scale_factor = 1.0 / scale_factor;
        double newScale = transform().m11() * scale_factor;
        double minScale = 0.1;
        double maxScale = 10.0;
        if (newScale < minScale || newScale > maxScale) {
            return;
        }
        scale(scale_factor, scale_factor);
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

QMap<QString, VertexItem *> GraphWidget::getVertices() const {
    return vertices;
}

std::vector<std::pair<VertexItem *, VertexItem *>> GraphWidget::getEdges() const {
    return edges;
}

QRectF GraphWidget::getSceneBounds() const {
    return scene->sceneRect(); // 获取场景的矩形边界
}

// 在GraphWidget中的calculateForces方法中应用四叉树
void calculateForces(GraphWidget *graphWidget) {
    auto nodesMap = graphWidget->getVertices();
    auto edges = graphWidget->getEdges();

    // 如果没有节点，直接返回
    if (nodesMap.isEmpty()) {
        return;
    }

    // 计算动态边界
    QRectF boundary = calculateDynamicBoundary(nodesMap);

    // 构建四叉树
    QuadTree qt(boundary, 1); // 容量设置为1，每个叶节点只包含一个顶点
    std::vector<VertexWithInfo *> vertexList;
    for (auto &key : nodesMap.keys()) {
        if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(nodesMap[key])) {
            qt.insert(vertex);
            vertexList.push_back(vertex);
        }
    }

    // 重置所有节点的力为零
    for (auto &vertex_info : vertexList) {
        vertex_info->setForce(QPointF(0, 0));
    }

    // 并行计算斥力
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // 默认线程数

    size_t totalVertices = vertexList.size();
    size_t chunkSize = (totalVertices + numThreads - 1) / numThreads;

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalVertices);

        if (startIdx >= endIdx)
            break;

        threads.emplace_back([&qt, &vertexList, startIdx, endIdx]() {
            for (size_t j = startIdx; j < endIdx; ++j) {
                VertexWithInfo *vertex = vertexList[j];
                QPointF force = qt.calculateForce(vertex);
                vertex->addForce(force);
            }
        });
    }

    // 等待所有线程完成
    for (auto &t : threads) {
        if (t.joinable())
            t.join();
    }

    // 计算吸引力（保持单线程或进一步优化）
    for (auto &edgePair : edges) {
        VertexWithInfo *vi = dynamic_cast<VertexWithInfo *>(edgePair.first);
        VertexWithInfo *vj = dynamic_cast<VertexWithInfo *>(edgePair.second);
        if (vi && vj) {
            QPointF delta = vi->pos() - vj->pos();
            double distance = std::max(std::hypot(delta.x(), delta.y()), 1.0);
            double attractiveForce = (distance * distance) / 50000.0; // 调整后的吸引力常数
            QPointF direction = delta / distance;
            vi->addForce(-attractiveForce * direction);
            vj->addForce(attractiveForce * direction);
        }
    }

    // 添加中心引力
    QPointF sceneCenter = graphWidget->scene->sceneRect().center();

    for (auto &vertex : vertexList) {
        double centerStrength = 0.05;
        QPointF delta = sceneCenter - vertex->pos();
        double distance = std::max(std::hypot(delta.x(), delta.y()), 0.01);
        QPointF centerForce = centerStrength * delta / distance;
        vertex->addForce(centerForce);
    }

    // 更新所有节点的位置
    //for (auto &vertex : vertexList) {   // ?开销很大
    //    vertex->applyForce(graphWidget->getSceneBounds());
    //}

    // 更新场景显示
    //graphWidget->scene->update();
}
