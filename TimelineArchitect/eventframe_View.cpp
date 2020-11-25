#include "eventframe_View.h"
#include "ui_eventframe_View.h"
#include <QMouseEvent>
#include <QColor>

EventFrame::EventFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{

    ui->setupUi(this);

}

EventFrame::~EventFrame()
{
    delete ui;
}


void EventFrame::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}


void EventFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - offset));
    }
}

void EventFrame::mouseReleaseEvent(QMouseEvent *event)
{
    auto par = (QFrame*) parent();
    auto pos = x();
    if (pos< 0)
    {
        this->move(mapToParent(event->pos() - QPoint(pos,0) - offset));
     }
    else
    {
        pos = pos+width() - par->width();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(pos,0) - offset));
    }
    pos = y();
    if(pos<0)
    {
        this->move(mapToParent(event->pos() + QPoint(0,-pos))- offset);
    }
    else
    {
        pos = pos+height() - par->height();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(0,pos) - offset));
    }
}
