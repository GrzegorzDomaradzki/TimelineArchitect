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

}

CentralFrame::~CentralFrame()
{
    foreach (auto event, _eventsViews)
    {
        delete event.second;
    }
    delete ui;
}

void CentralFrame::AddEvent(Event *event)
{

    auto newFrame = new EventFrame(_generator++,event,nullptr);
    newFrame->setParent(this);
    newFrame->setFrameShape(QFrame::StyledPanel);
    newFrame->setFrameShadow(QFrame::Raised);
    _eventsViews.insert(std::pair<unsigned,EventFrame*>(_generator,newFrame));
    newFrame->move(event->realPos*_resolution-_relativePosition,height()/2);
    newFrame->show();
    connect(newFrame,SIGNAL(NoRepaint()),this,SLOT(OnStopDraw()));
    Redraw();
}

int CentralFrame::GetResolution()
{
    return _resolution;
}

void CentralFrame::SetResolution(int res)
{
    if(_resolution>1) _resolution = res;
}

void CentralFrame::Redraw()
{
    this->repaint();
}

void CentralFrame::UpdateTimelineData()
{
    _end = timeEngine->getLength()*_resolution;
    Redraw();
}

void CentralFrame::paintEvent(QPaintEvent *event)
{
    if (_end==0) return;
    auto painter = new QPainter(this);
    painter->setPen(QPen(color,3));
    painter->setFont(QFont("Arial",12));
    auto startPos = 0;
    int endPos;
    if (width()>_end-(int)_relativePosition) endPos=_end-_relativePosition;
    else endPos=width();
    auto mid = height()/2;
    painter->drawLine(startPos,mid,endPos,mid);
    auto now = startPos - _relativePosition % _resolution;
    unsigned i = 1;
    auto ahead = (endPos-now)/_resolution+1;
    int toBreak = GetDist(ahead,_relativePosition);
    if (endPos+4*_resolution>_end-(int)_relativePosition)
    {
        PrintArrow(_end-_relativePosition+_resolution,painter);
        endPos=_end-(int)_relativePosition-4*_resolution;
    }
    if (_relativePosition<3) painter->drawLine(now+1,height(),now+1,0);
    now= _resolution;
    PrintDate(now,painter,2);
    while ((int)now<endPos)
    {
        if (!toBreak) toBreak = Reload(now,ahead,painter);
        else
        {
            if (i % 5 == 0)
            {
                if(i % 15 == 0)
                {
                    painter->drawLine(now,mid-2.5*_resolution,now,mid+2.5*_resolution);
                    PrintDate(now,painter);
                }
                else painter->drawLine(now,mid-2*_resolution,now,mid+2*_resolution);
            }
            else painter->drawLine(now,mid-_resolution,now,mid+_resolution);
        }
        now+=_resolution;
        i++;
        ahead--;
        toBreak--;
    }
    PrintEventLines(painter);
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
    if (_end<3*_resolution) return;
    if(length<=0) _relativePosition = 0;
    else if(length>_end-3*_resolution) _relativePosition = _end-3*_resolution;
    else _relativePosition += toAdd.x();
    //qDebug() << _relativePosition;
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

void CentralFrame::PrintDate(unsigned location, QPainter* painter,int ascended)
{
    auto old = painter->pen();
    painter->setPen(Qt::black);
    if (ascended == 2) painter->drawText(location,height()/2+_resolution*8,1,1,Qt::AlignLeft|Qt::TextDontClip,GetNext());
    else if (ascended) painter->drawText(location+(ascended*_resolution*4),height()/2+_resolution*6,1,1,Qt::AlignCenter|Qt::TextDontClip,GetNext());
    else painter->drawText(location,height()/2-_resolution*3.5,1,1,Qt::AlignCenter|Qt::TextDontClip,GetNext());
    painter->setPen(old);
}

void CentralFrame::PrintArrow(unsigned start,QPainter* painter)
{
    QPainterPath path;
    path.moveTo(start,height()/2);
    path.lineTo(start-_resolution*4,height()/2+_resolution*2);
    path.lineTo(start-_resolution*4,height()/2-_resolution*2);
    path.lineTo(start,height()/2);
    painter->fillPath (path, QBrush (QColor (color)));
    auto old = painter->pen();
    painter->setPen(Qt::black);
   painter->drawText(start+3*_resolution,height()/2+_resolution*3.5,1,1,Qt::AlignCenter|Qt::TextDontClip,_toWrite[_toWrite.size()-1]);
   painter->setPen(old);
}

void CentralFrame::PrintEventLines(QPainter *painter)
{
    foreach(auto event, _eventsViews)
    {
        auto frame = event.second;
        qDebug()<< *frame->real;
        auto y = frame->y()+frame->height()/2;
        auto x =*frame->real*_resolution - _relativePosition;
        painter->drawLine(x,height()/2,x,y);
        painter->drawLine(frame->x()+10,y,x,y);
        if (*frame->dual)
        {
            x = _relativePosition - *frame->dual*_resolution;
            painter->drawLine(x,height()/2,x,y);
            painter->drawLine(frame->x()+10,y,x,y);
        }
    }
}

int CentralFrame::GetDist(int ahead, int position)
{
    static int next = -1;
    _bookmark = -1;
    _toWrite.clear();
    if (position!=-1)
    {
        next = timeEngine->findRealPosition(position/_resolution);
    }
    else if (timeEngine->TimelineCount()==next) return 9999;
    auto timeline = timeEngine->GetTimeline(next);
    next++;
    return  timeline->StepsAchead(position/_resolution,ahead,&_toWrite);
}

int CentralFrame::Reload(int now, int ahead, QPainter* painter)
{
   painter->drawLine(now,height()/2-4*_resolution,now,height()/2+4*_resolution);
   PrintDate(now,painter,-1);
   int dist = GetDist(ahead);
   PrintDate(now,painter,1);
   return dist;
}

QString CentralFrame::GetNext()
{
    if (_toWrite.size()==0) return "dd.mm.yyyy";
    if (_bookmark+1<(int)_toWrite.size()) _bookmark++;
    return _toWrite[_bookmark];
}

void CentralFrame::OnStopDraw()
{
    qDebug()<<"in!";
}

void CentralFrame::OnResumeDraw()
{

}

void CentralFrame::OnForgotten(int id)
{

}



