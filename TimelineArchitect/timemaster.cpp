#include "timemaster.h"

TimeMaster::TimeMaster(QObject *parent) : QObject(parent)
{
    tags = new Tags();
    _currID = 0;
}

TimeMaster::~TimeMaster()
{
    foreach (auto timeline, _timelines) {
        delete timeline;
    }

    foreach (auto event, _events) {
        delete event;
    }
    delete tags;
}

int TimeMaster::EventCount()
{
    return _events.size();
}

int TimeMaster::TimelineCount()
{
    return _timelines.size();
}




int TimeMaster::findPosition(QDate date)
{
    if ((int)_timelines.size()==-1 || _timelines[0]->GetStart()>=date) return -1;
    int i = 0;
    while (i<(int)_timelines.size() && _timelines[i]->GetEnd()<=date) i++;
    return i;
}

int TimeMaster::findEventPosition(QDate date)
{
    int i = 0;
    while (i<(int)_events.size() && _events[i]->GetDateStart()<date) i++;
    return i;
}

int TimeMaster::AddTimeline(Timeline* timeline)
{
    if (_timelines.size()==0)
    {
        timeline->NumberInLine=0;
        timeline->SetStartRel(0);
        _timelines.push_back(timeline);
        return 0;
    }
    auto i = findPosition(timeline->GetStart());
    Timeline* intercut;
    QDate intercutEnd;
    if (i==-1 || i == (int)_timelines.size()) intercut = nullptr;
    else
    {
        intercut = _timelines[i];
        intercutEnd = intercut->GetEnd();
    }
    if (i==-1) timeline->SetStartRel(0);
    else
    {
        if (i == (int)_timelines.size()) i--;
        _timelines[i]->ShrinkRight(timeline->GetStart());
        timeline->Grow(_timelines[i]->GetEnd());
        timeline->SetStartRel(_timelines[i]->GetEndRel());
    }
    i++;
    timeline->NumberInLine=i;
    //kasowanie
    if (!intercut || intercutEnd<timeline->GetEnd())
    {
        intercut=nullptr;
        while(i<(int)_timelines.size())
        {
            if(_timelines[i]->GetEnd()<=timeline->GetEnd())
            {
                delete _timelines[i];
               _timelines.erase(_timelines.begin()+i);
            }
            else break;
        }
    }
    _timelines.insert(_timelines.begin()+i,timeline);
    //kurczenie
    i++;
    if (i!=(int)_timelines.size())
    {
        _timelines[i]->ShrinkLeft(timeline->GetStart());
        timeline->Grow(_timelines[i]->GetEnd());
        //edycja
        for (;i<(int)_timelines.size();i++)
        {
            _timelines[i]->NumberInLine=i;
            _timelines[i]->SetStartRel(_timelines[i-1]->GetEndRel());
        }
    }
    i = timeline->NumberInLine;
    if (intercut!=nullptr) AddTimeline(new Timeline(timeline->GetEnd(),intercutEnd,intercut->GetStep(),this));
    if (i!=0 && timeline->GetStep()==_timelines[i-1]->GetStep())
    {
        if (i!=(int)_timelines.size()-1 && timeline->GetStep()==_timelines[i+1]->GetStep()) AddTimeline(new Timeline (_timelines[i-1]->GetStart(),_timelines[i+1]->GetEnd(),timeline->GetStep(),this));
        else AddTimeline(new Timeline (_timelines[i-1]->GetStart(),timeline->GetEnd(),timeline->GetStep(),this));
    }
    else if (i!=(int)_timelines.size()-1 && timeline->GetStep()==_timelines[i+1]->GetStep()) AddTimeline(new Timeline (timeline->GetStart(),_timelines[i+1]->GetEnd(),timeline->GetStep(),this));
    //UpdateEventsPointers();
    return 0;
}

int TimeMaster::AddEvent(Event* event, QString &info)
{
    info = "";
    int timelinePos = findPosition(event->GetDateStart());
    int endPos;
    if (timelinePos==-1 || timelinePos==(int)_timelines.size() ||(event->isDual && (endPos=findPosition(event->GetDateEnd()))==(int)_timelines.size() ))
    {
        info = "Date outside borders, resize timeline borders!";
        return -1;
    }
    auto pos = findEventPosition(event->GetDateStart());
    _events.insert(_events.begin()+pos,event);
    foreach (auto oldPos, _events_id)
    {
        if ((int)oldPos<=pos) oldPos++;
    }
    _events_id.push_back(pos);
    event->realPos = _timelines[timelinePos]->GetReal(event->GetDateStart());
    if (event->isDual) event->isDual = _timelines[endPos]->GetReal(event->GetDateEnd());
    if (EventCount()>0)
    event->id = _currID++;
    return pos;
}

bool TimeMaster::ShiftEvent(Event &event, QString &info)
{

}

void TimeMaster::updateLength()
{
    unsigned length = 0;
    foreach(auto timeline, _timelines)
    {
        length+=timeline->GetLength();
    }
    _length = length;
}

int TimeMaster::findRealPosition(unsigned step)
{
    int i = 0;
    foreach (auto timeline, _timelines)
    {
        if (step<=timeline->GetStartRel()) return i;
        ++i;
    }
    return i-1;
}

Timeline *TimeMaster::GetTimeline(int i)
{
    return _timelines[i];
}

unsigned TimeMaster::getLength()
{
    return _length;
}
