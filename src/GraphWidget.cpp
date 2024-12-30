#include "GraphWidget.h"
#include <QWheelEvent>
#include <QTimer>
#include "EdgeItem.h"
#include "VertexWithInfo.h"
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <limits>
#include <spdlog/spdlog.h>

namespace {
    QRectF calculateDynamicBoundary(const QMap<QString, VertexItem *> &nodes) {
        if (nodes.isEmpty()) {
            // 如果没有节点，返回一个默认边界
            return {-1000, -1000, 2000, 2000};
        }

        // 初始化最小和最大值
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double maxY = std::numeric_limits<double>::lowest();

        // 遍历所有节点，找出最小和最大位置
        for (auto node : nodes) {
            if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
                QPointF pos = vertex->pos();
                minX = std::min(minX, pos.x());
                minY = std::min(minY, pos.y());
                maxX = std::max(maxX, pos.x());
                maxY = std::max(maxY, pos.y());
            }
        }

        // 给边界添加一定的缓冲区以避免节点靠近边界时超出
        double padding = 200.0; // 缓冲区大小
        return {
            minX - padding, minY - padding,
            (maxX - minX) + 2 * padding,
            (maxY - minY) + 2 * padding
        };
    }
}

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent) {
    // 初始化场景
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿
    // 设置场景的边界
    QRectF dynamicBoundary = calculateDynamicBoundary(this->getVertices()); // 根据节点计算边界
    scene->setSceneRect(dynamicBoundary);

    // 使用定时器定时执行力导向布局算法
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {
        // 每次定时器触发时，计算节点间的力并更新位置
        calculateForces(this);
        auto nodes = this->getVertices();
        // 更新节点位置（使用适当的时间步长和阻尼）

        for (auto node : nodes) {
            if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
                double damping = 0.85;
                double timeStep = 0.05;
                QPointF force = vertex->getForce();
                QPointF velocity = vertex->getVelocity();

                // 更新速度
                velocity = velocity * damping + force * timeStep;

                // 更新位置
                QPointF newPos = vertex->pos() + velocity;

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

std::vector<std::pair<VertexItem *, VertexItem *>> GraphWidget::getEdges() {
    return edges;
}

void calculateForces(GraphWidget *graphWidget) {
    auto nodes = graphWidget->getVertices();
    auto edges = graphWidget->getEdges();

    // 重置所有节点的力为零
    for (auto &node : nodes) {
        if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
            vertex->setForce(QPointF(0, 0)); // 初始化力为零
        }
    }

    // 获取所有节点指针到一个列表，方便并行处理
    std::vector<VertexWithInfo *> vertexList;
    for (auto &node : nodes) {
        if (VertexWithInfo *vertex = dynamic_cast<VertexWithInfo *>(node)) {
            vertexList.push_back(vertex);
        }
    }

    // 定义线程数量（根据硬件调整）
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // 默认线程数

    // 分割任务
    std::vector<std::thread> threads;
    size_t totalVertices = vertexList.size();
    size_t chunkSize = (totalVertices + numThreads - 1) / numThreads; // 向上取整

    // 创建一个临时数组来存储每个线程的计算结果
    std::vector<QPointF> forces(totalVertices, QPointF(0, 0));

    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalVertices);

        if (startIdx >= endIdx)
            break; // 任务分配完毕

        threads.emplace_back([startIdx, endIdx, &vertexList, &nodes, &forces]() {
            for (size_t j = startIdx; j < endIdx; ++j) {
                VertexWithInfo *vi = vertexList[j];
                QPointF totalForce(0, 0);
                for (auto &vj_node : nodes) {
                    if (vj_node == vi) continue;
                    if (VertexWithInfo *vj = dynamic_cast<VertexWithInfo *>(vj_node)) {
                        QPointF delta = vi->pos() - vj->pos();
                        double distance = std::max(std::hypot(delta.x(), delta.y()), 1.0);
                        double repulsiveForce = 10000.0 / (distance * distance); // 可调参数
                        QPointF direction = delta / distance;
                        QPointF force = repulsiveForce * direction;
                        totalForce += force;
                    }
                }
                forces[j] = totalForce;
            }
        });
    }

    // 等待所有线程完成
    for (auto &t : threads) {
        if (t.joinable())
            t.join();
    }

    // 将计算结果应用到节点
    for (size_t j = 0; j < vertexList.size(); ++j) {
        vertexList[j]->setForce(forces[j]);
    }

    // 计算吸引力（保持单线程）
    for (auto &edge : edges) {
        VertexWithInfo *vi = dynamic_cast<VertexWithInfo *>(edge.first);
        VertexWithInfo *vj = dynamic_cast<VertexWithInfo *>(edge.second);
        if (vi && vj) {
            // 计算两个节点之间的位移向量和欧几里得距离
            QPointF delta = vi->pos() - vj->pos();
            // 使用欧几里得距离，避免为 0
            double distance = std::max(std::hypot(delta.x(), delta.y()), 1.0);
            // 计算吸引力，通常吸引力与距离呈线性关系，这里设为 F = k * d
            double attractiveForce = (distance * distance) / 1000.0; // k 可以根据图大小调整
            QPointF direction = delta / distance;
            // 更新节点的力（相连节点向彼此靠拢）
            vi->addForce(-attractiveForce * direction);
            vj->addForce(attractiveForce * direction);
        }
    }
}
