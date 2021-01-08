#include "centralframe_View.h"
#include "ui_centralframe_View.h"
#include <QDebug>



CentralFrame::CentralFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CentralFrame)
{
    ui->setupUi(this);
    _generator = 0;
    color = Qt::red;
    _relativePosition = 0;
    _resolution = 10;
    _end = 0;
    auto TestFrame1 =new EventFrame(_generator++, this);
    TestFrame1->setFrameShape(QFrame::StyledPanel);
    TestFrame1->setFrameShadow(QFrame::Raised);
    _eventsViews.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame1));

    auto TestFrame2 = new EventFrame(_generator++,this);
    TestFrame2->setFrameShape(QFrame::StyledPanel);
    TestFrame2->setFrameShadow(QFrame::Raised);
    _eventsViews.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame2));

    auto TestFrame3 =new EventFrame(_generator++,this);
    TestFrame3->setFrameShape(QFrame::StyledPanel);
    TestFrame3->setFrameShadow(QFrame::Raised);
    _eventsViews.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame3));


}

CentralFrame::~CentralFrame()
{
    delete ui;
}

void CentralFrame::AddEvent(Event *event)
{
    auto newFrame = new EventFrame(_generator++,event,this);
    newFrame->setFrameShape(QFrame::StyledPanel);
    newFrame->setFrameShadow(QFrame::Raised);
    _eventsViews.insert(std::pair<unsigned,EventFrame*>(_generator,newFrame));
}

int CentralFrame::GetResolution()
{
    return _resolution;
}

void CentralFrame::SetResolution(int res)
{
    if(_resolution>1) _resolution = res;
}

void CentralFrame::UpdateTimelineData()
{

}

void CentralFrame::paintEvent(QPaintEvent *event)
{
    auto painter = new QPainter(this);
    painter->setPen(QPen(color,3));
    painter->setFont(QFont("Arial",12));
    auto startPos = 0;
    int endPos;
    if (width()>_end-(int)_relativePosition) endPos=_end-_relativePosition;
    else endPos=width();
    auto mid = height()/2;
    //if not start and end
    painter->drawLine(startPos,mid,endPos,mid);

    auto now = startPos - _relativePosition % _resolution;
    unsigned i = _relativePosition/_resolution;
    if (_relativePosition<3) painter->drawLine(now+1,height(),now+1,0);
    while ((int)now<endPos)
    {
        if (i % 5 == 0)
        {
            painter->drawLine(now,mid-2*_resolution,now,mid+2*_resolution);
        }
        else painter->drawLine(now,mid-_resolution,now,mid+_resolution);
        now+=_resolution;
        i++;
    }
    delete painter;
}

void CentralFrame::mousePressEvent(QMouseEvent *event)
{
    _offset = event->pos();
    this->raise();
}


void CentralFrame::mouseMoveEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    pos.setY(_offset.y());
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(pos - _offset));
    }
}

void CentralFrame::mouseReleaseEvent(QMouseEvent *event)
{
    auto par = (QFrame*) parent();
    auto pos = x();
    auto eventPos = event->pos();
    eventPos.setY(_offset.y());
    QPoint toAdd;

    pos = pos+width() - par->width();
    toAdd = eventPos - QPoint(pos,0) - _offset;
    this->move(mapToParent(toAdd));
    auto old = _relativePosition;
    auto length = toAdd.x()+(int)_relativePosition;
    if(length<=0) _relativePosition = 0;
    else if(length>_end-_resolution) _relativePosition = _end-_resolution;
    else _relativePosition += toAdd.x();
    qDebug() << _relativePosition;
    if (old!=_relativePosition) moveEvents(old-_relativePosition);
}

void CentralFrame::moveEvents(int diff)
{
    EventFrame* event;
    foreach (auto map, _eventsViews)
    {
        event = map.second;
        event->move(diff+event->x(),event->y());
    }
}



