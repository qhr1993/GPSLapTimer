#include "cwidget.h"
#include <QKeyEvent>
#include <QDebug>

CWidget::CWidget(QWidget *parent):QWidget(parent)
{

}

CWidget::~CWidget()
{

}

void CWidget::keyPressEvent(QKeyEvent *e)
{
    qWarning()<<"rec key";
    qWarning()<<state;
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
    break;
    case Qt::Key_Return:
        emit stateChangeRequest(state,0);
        break;
    default:
        break;
    }
}

void CWidget::keyReleaseEvent(QKeyEvent *e)
{

}
