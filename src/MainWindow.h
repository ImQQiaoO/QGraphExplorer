#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Search.h"
#include "GraphWidget.h"

class QLineEdit;
class AddItems;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    GraphWidget *graphWidget;
    std::unique_ptr<AddItems> add_items_;
    std::unique_ptr<Search> search_;

};

#endif // MAINWINDOW_H
