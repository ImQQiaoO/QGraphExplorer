#ifndef NODEINFOPOPUP
#define NODEINFOPOPUP

#include <QLabel>

class NodeInfoPopup : public QWidget {
public:
    explicit NodeInfoPopup(const QString &info, QWidget *parent = nullptr);
};

#endif // NODEINFOPOPUP