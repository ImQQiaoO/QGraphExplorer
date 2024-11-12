#ifndef NODEINFOPOPUP_H
#define NODEINFOPOPUP_H

#include <QLabel>

class VertexInfoPopup : public QWidget {
public:
    explicit VertexInfoPopup(const QString &info, QWidget *parent = nullptr);
};

#endif // NODEINFOPOPUP_H