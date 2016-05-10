#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CPushButton(QWidget * parent = 0);
    ~CPushButton();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    int state;
signals:
    void stateChangeRequest(int currentStatus, int keyIndicator);
    //keyIndicator esc=-1 enter=0 up=1 dn=2 left=3 right=4
};

#endif // CPUSHBUTTON_H
