#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "GraphWidget.h"

class AddItems;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    GraphWidget *graphWidget;
    std::unique_ptr<AddItems> add_items_;
};

#endif // MAINWINDOW_H
