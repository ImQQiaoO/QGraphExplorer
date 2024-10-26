#ifndef NODEINFOPOPUP_H
#define NODEINFOPOPUP_H

#include <QLabel>

class NodeInfoPopup : public QWidget {
public:
    explicit NodeInfoPopup(const QString &info, QWidget *parent = nullptr);
};

#endif // NODEINFOPOPUP_H