#include"NodeInfoPopup.h"
#include <QVBoxLayout>

NodeInfoPopup::NodeInfoPopup(const QString &info, QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::ToolTip);  // 设置窗口类型为 ToolTip，这样窗口没有边框
    setStyleSheet("background-color: lightblue; border: 1px solid gray;");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(info, this);
    layout->addWidget(label);
    setLayout(layout);
}
