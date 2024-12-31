#pragma once
#include <QLineEdit>
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "GraphWidget.h"
#include "VertexItem.h"
#include "utils/Locale.hpp"

class Search : public QObject {
    QLineEdit *lineEdit;
public:
    Search(GraphWidget *graphWidget);
    static QString target_string;

private slots:
    void return_pressed(GraphWidget *graphWidget) const;
};


