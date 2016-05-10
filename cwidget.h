#ifndef CWIDGET_H
#define CWIDGET_H

#include <QWidget>

class CWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CWidget(QWidget *parent=0);
    ~CWidget();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    int state;
signals:
    void stateChangeRequest(int currentStatus, int keyIndicator);
    //keyIndicator esc=-1 enter=0 up=1 dn=2 left=3 right=4
};

#endif // CWIDGET_H
