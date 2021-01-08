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

int TimeMaster::AskForDate(QDate start, QDate end)
{
    int pos = findPosition(0,_timelines.size()-1,start);
    if (pos==-1) return -1;
    auto x = _timelines[pos];
    if (x!=nullptr && x->Contains(start) && x->Contains(end)) return x->NumberInLine;
    return -1;
}

int TimeMaster::AskForDate(QDate start)
{
    int pos = findPosition(0,_timelines.size()-1,start);
    if (pos==-1) return -1;
    auto x = _timelines[pos];
    if (x!=nullptr && x->Contains(start)) return x->NumberInLine;
    return -1;
}



int TimeMaster::findPosition(int start, int end, QDate date)
{
    if (start==end)return start;
    int to_check = (start+end)/2;
    int dist = _timelines[to_check]->Distance(date);
    if (dist==0) return to_check;
    if (dist<0) return findPosition(start,to_check-1,date);
    return findPosition(to_check+1,end,date);

}

int TimeMaster::findEventPosition(int start, int end, QDate date)
{
    if (start==end) return start;
    int to_check = (start+end)/2;
    QDate event_date = _events[to_check]->GetDateStart();
    if (event_date==date) return to_check;
    if (event_date>date)return findPosition(start,to_check-1,date);
    return findPosition(to_check+1,end,date);
}

int TimeMaster::AddTimeline(Timeline* timeline, int& succes,QString &info)
{
    succes=1;
    if (_timelines.empty())
    {
        timeline->NumberInLine = 0;
        _timelines.push_back(timeline);
        return 0;
    }
    int pos = findPosition(0,_timelines.size()-1,timeline->GetStart());
    if (pos == -1)
    {
       auto first_event = _timelines[0];
       if (first_event->Distance(timeline->GetEnd()) >= 0)
       {
           info = "timeline has colision with otherone, force new timeline and resize others?";
           succes=-1;
           return -1;
       }
       timeline->NumberInLine = 0;
    }
    else
    {
        auto first_event = _timelines[pos];
        auto second_event = _timelines[pos+1];
        if (first_event->Distance(timeline->GetStart())==0 || second_event->Distance(timeline->GetEnd()) >= 0)
        {
            info = "timeline has colision with otherone, force new timeline and resize others?";
            succes=-1;
            return pos;
        }
        timeline->NumberInLine = pos+1;
    }
    for (unsigned i = pos+1; i<_timelines.size(); i++)
    {
        _timelines[i]->NumberInLine++;
    }
    _timelines.insert(_timelines.begin()+pos,timeline);
    return timeline->NumberInLine;
}

int TimeMaster::AddEvent(Event* event, QString &info)
{
    info = "";
    int timeline;
    if (event->IsBinary())
    {
        timeline = AskForDate(event->GetDateStart());
        if (timeline == -1)
        {
            info = "Date outside timeline, change timeline borders!";
            return -1;
        }
    }
    else
    {
        timeline = AskForDate(event->GetDateStart());
        if (timeline == AskForDate(event->GetDateEnd()) && timeline !=-1)
        {
            info = "Date outside timeline, change timeline borders!";
            return -1;
        }
    }
    int pos;
    if (EventCount()>0)
    pos = findEventPosition(0,_events.size()-1,event->GetDateStart())+1;
    else pos = 0;
    for (unsigned i = pos+1; i<_events.size();i++)
    {
        _events[i]->NumberInLine++;
    }
    _events.insert(_events.begin()+pos,event);
    _events_id.insert(_events_id.begin()+pos,_currID);
    event->id = _currID;
    return event->NumberInLine = pos+1;
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

unsigned TimeMaster::getLength()
{
    return _length;
}
