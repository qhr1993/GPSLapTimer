#include "cpushbutton.h"
#include <QKeyEvent>

CPushButton::CPushButton(QWidget * parent):QPushButton(parent)
{

}

CPushButton::~CPushButton()
{

}

void CPushButton::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Left:
    emit stateChangeRequest(state,3);
    break;
    case Qt::Key_Right:
    emit stateChangeRequest(state,4);
    break;
    case Qt::Key_Down:
        emit stateChangeRequest(state,2);
    break;
    case Qt::Key_Up:
        emit stateChangeRequest(state,1);
    break;
    case Qt::Key_Escape:
        emit stateChangeRequest(state,-1);
    break;
    case Qt::Key_Enter:
        click();
        break;
    default:
        break;
    }
}
void CPushButton::keyReleaseEvent(QKeyEvent *e)
{

}
