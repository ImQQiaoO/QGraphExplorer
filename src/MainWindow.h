#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "GraphWidget.h"

class QLineEdit;
class AddItems;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_returnPressed();  // 槽函数，用于捕获回车键事件

private:
    GraphWidget *graphWidget;
    std::unique_ptr<AddItems> add_items_;

    QLineEdit *lineEdit;  // 新添加的 QLineEdit 控件
};

#endif // MAINWINDOW_H
