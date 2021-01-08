#include "eventframe_View.h"
#include "ui_eventframe_View.h"


EventFrame::EventFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{

    ui->setupUi(this);

}

EventFrame::EventFrame(unsigned new_id, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{
    ui->setupUi(this);
    _id = new_id;
}

EventFrame::EventFrame(unsigned new_id, Event *event, QWidget *parent):
    QFrame(parent),
    ui(new Ui::EventFrame)
{
    ui->setupUi(this);
    _id = new_id;
    _event = event;
    ui->TitleLab->setText(event->name);
    ui->DescLab->setText(event->text);
    if(event->IsBinary()) ui->DateLab->setText(event->GetDateStart().toString());
    else
    {
        QString date = event->GetDateStart().toString();
        date += "-" + event->GetDateEnd().toString();
        ui->DateLab->setText(date);
    }
    this->raise();
}

EventFrame::~EventFrame()
{
    delete ui;
}


void EventFrame::mousePressEvent(QMouseEvent *event)
{
    _offset = event->pos();
    this->raise();
}


void EventFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - _offset));
    }
}

void EventFrame::mouseReleaseEvent(QMouseEvent *event)
{
    auto par = (QFrame*) parent();
    auto pos = x();
    if (pos< 0)
    {
        this->move(mapToParent(event->pos() - QPoint(pos,0) - _offset));
     }
    else
    {
        pos = pos+width() - par->width();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(pos,0) - _offset));
    }
    pos = y();
    if(pos<0)
    {
        this->move(mapToParent(event->pos() + QPoint(0,-pos))- _offset);
    }
    else
    {
        pos = pos+height() - par->height();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(0,pos) - _offset));
    }
}
