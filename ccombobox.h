#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <QWidget>
#include <QComboBox>

class CComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CComboBox(QWidget * parent = 0);
    ~CComboBox();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    int state;
signals:
    void stateChangeRequest(int currentStatus, int keyIndicator);
};

#endif // CCOMBOBOX_H
