#include "ccombobox.h"
#include <QKeyEvent>
#include <QDebug>

CComboBox::CComboBox(QWidget * parent):QComboBox(parent)
{

}

CComboBox::~CComboBox()
{

}

void CComboBox::keyPressEvent(QKeyEvent *e)
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
    case Qt::Key_Return:
        showPopup();
        break;
    default:
        break;
    }
}

void CComboBox::keyReleaseEvent(QKeyEvent *e)
{

}

