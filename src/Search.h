#pragma once
#include <QLineEdit>
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "GraphWidget.h"
#include "VertexItem.h"
#include "utils/Locale.hpp"

class Search : public QObject {
    Q_OBJECT

public:
    Search(GraphWidget *graphWidget);
    static QString target_string;

private:
    QLineEdit *lineEdit;
    QToolButton *dropDownButton;
    QString currentCategory;
    std::vector<QString> categoryList;

private slots:
    void return_pressed(GraphWidget *graphWidget) const;
    void dropdown_option_selected(const QAction *action);
    void button_pressed(bool checked);
};


