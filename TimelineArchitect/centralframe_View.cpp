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
    _noPaint = false;
    _eventsViews = QMap<int,EventFrame*>();
    _selectedColor = Qt::red;
    auto pal = this->palette();
    pal.setColor(QPalette::Background,Qt::white);
    this->setPalette(pal);
}

CentralFrame::~CentralFrame()
{
    foreach (auto event, _eventsViews)
    {
        delete event;
    }
    delete ui;
}

void CentralFrame::AddEvent(Event *event)
{

    auto newFrame = new EventFrame(_generator,event,nullptr);
    newFrame->setParent(this);
    _eventsViews[_generator++] = newFrame;
    newFrame->move(event->realPos*_resolution-_relativePosition,height()/2);
    newFrame->SetMarkedColor(_selectedColor);
    newFrame->setAutoFillBackground(true);
    newFrame->show();
    connect(newFrame,&EventFrame::RewerseRepaint,this,&CentralFrame::OnRewersePaint);
    connect(newFrame,&EventFrame::AddMe,this,&CentralFrame::OnAddMe);
    Redraw();
}

int CentralFrame::GetResolution()
{
    return _resolution;
}

void CentralFrame::SetResolution(int res)
{
    if(_resolution>1) _resolution = res;
    else return;
    _end/=_resolution;
    _end*=res;
    foreach (auto frame, _eventsViews)
    {
        frame->move(((frame->x()+_relativePosition)/(double)_resolution*res)-_relativePosition,frame->y());
    }

    Redraw();
}

void CentralFrame::Redraw()
{
    this->repaint();
}

void CentralFrame::UpdateTimelineData()
{
    _end = timeEngine->getLength()*_resolution;
    foreach (auto frame, _eventsViews)
    {
        frame->move(*(frame->real)*_resolution-_relativePosition,frame->y());
    }
    Redraw();
}

void CentralFrame::EraseSelected()
{
    if (_selected.size()>0)
    {
        auto reply = QMessageBox::question(this, "Erase events", QString("Do you want to delete ") + QString::number(_selected.size()) + " events?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply!=QMessageBox::Yes) return;
        foreach(auto selected, _selected)
        {
            _eventsViews[selected]->SayGoodbye();
            _eventsViews.remove(selected);
        }
        _selected.clear();
        Redraw();
    }
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
    if (_relativePosition<3) painter->drawLine(now+1,height(),now+1,0);
    now= _resolution;
    PrintDate(now,painter,2);
    bool control = false;
    if (endPos+4*_resolution>_end-(int)_relativePosition)
    {
        endPos=_end-(int)_relativePosition-4*_resolution;
        control = true;
    }
    while ((int)now<endPos)
    {
        if (!toBreak)
        {
            toBreak = Reload(now,ahead,painter);
            i = 1;
        }
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
    if (control) PrintArrow(_end-_relativePosition+_resolution,painter);
    if (!_noPaint) PrintEventLines(painter);
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
    if (old!=_relativePosition) moveEvents(old-_relativePosition);
}

void CentralFrame::resizeEvent(QResizeEvent *event)
{
    //double ratio =  (double)event->size().height()/event->oldSize().height();
    foreach (auto frame, _eventsViews)
    {
        auto y = (double)frame->y()/event->oldSize().height()*event->size().height();
        if (y<0) y = 0;
        frame->move(frame->x(),y);
    }
}

void CentralFrame::moveEvents(int diff)
{
    foreach (auto event, _eventsViews)
    {
        event->move(diff+event->x(),event->y());
    }
}

void CentralFrame::PrintDate(unsigned location, QPainter* painter,int ascended)
{
    auto old = painter->pen();
    painter->setPen(Qt::black);
    if (ascended == 2) painter->drawText(location,height()/2+_resolution*8,1,1,Qt::AlignLeft|Qt::TextDontClip,GetNext());
    else if (ascended) painter->drawText(location+(ascended*_resolution*4),height()/2+_resolution*6*ascended,1,1,Qt::AlignCenter|Qt::TextDontClip,GetNext());
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
        if (event->isHidden()) continue;
        painter->setPen(QPen(event->GetColor(),3));
        auto y = event->y()+event->height()/2;
        auto x =*event->real*_resolution - _relativePosition;
        painter->drawLine(x,height()/2,x,y);
        painter->drawLine(event->x()+10,y,x,y);
        if (*event->dual)
        {
            x = *event->dual*_resolution - _relativePosition;
            painter->drawLine(x,height()/2,x,y);
            painter->drawLine(event->x()+10,y,x,y);
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

void CentralFrame::UnselectAll()
{
    foreach (auto select, _selected) {
        _eventsViews[select]->ChangeRadio();
    }
    _selected.clear();
}

void CentralFrame::HideGiven(std::vector<unsigned> IDs)
{
    foreach (auto id, IDs)
    {
        OnAddMe(id,0);
        _eventsViews[id]->hide();
    }
    Redraw();
}

void CentralFrame::ShowGiven(std::vector<unsigned> IDs)
{
    foreach (auto id, IDs) _eventsViews[id]->show();
    Redraw();
}

void CentralFrame::ShowAll()
{
    foreach (auto event, _eventsViews) event->show();
    Redraw();
}

void CentralFrame::Purge()
{
    _selected.clear();
    _generator = 0;
    foreach (auto event, _eventsViews) delete event;
    _eventsViews.clear();
}

void CentralFrame::SetSelectedColor(QColor color)
{
    _selectedColor = color;
    foreach (auto frame, _eventsViews) frame->SetMarkedColor(color);
}

QColor CentralFrame::GetSelectedColor()
{
    return _selectedColor;
}

QString CentralFrame::GetNext()
{
    if (_toWrite.size()==0) return "dd.mm.yyyy";
    if (_bookmark+1<(int)_toWrite.size()) _bookmark++;
    return _toWrite[_bookmark];
}

void CentralFrame::OnRewersePaint()
{
    if (!(_noPaint = !_noPaint)) this->repaint();
}



void CentralFrame::OnAddMe(unsigned id, bool add)
{
    if(add)
    {
        if (!_eventsViews[id]->isHidden()) _selected.push_back(id);
        return;
    }
    for (auto iterator = _selected.begin();iterator!=_selected.end(); iterator++) if (*iterator == id)
    {
        _selected.erase(iterator);
        break;
    };
}



