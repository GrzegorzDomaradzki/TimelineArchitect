#include "timemaster.h"

TimeMaster::TimeMaster(QObject *parent) : QObject(parent)
{
    tags = new Tags();
    Filename = "";
    connect(tags,&Tags::NameChange,this,&TimeMaster::OnNameChange);
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
    if ((int)_timelines.size()==0 || _timelines[0]->GetStart()>=date) return -1;
    int i = 0;
    while (i<(int)_timelines.size() && _timelines[i]->GetEnd()<date) i++;
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
    static int control = 0;
    control++;
    if (_timelines.size()==0)
    {
        timeline->NumberInLine=0;
        timeline->SetStartRel(0);
        _timelines.push_back(timeline);
        control--;
        return 0;
    }
    auto i = findPosition(timeline->GetStart());
    Timeline* intercut = nullptr;
    QDate intercutEnd;
    if (i==-1 || i == (int)_timelines.size());
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
        //timeline->Grow(_timelines[i]->GetEnd());
        timeline->SetStartRel(_timelines[i]->GetEndRel());
    }
    i++;
    timeline->NumberInLine=i;
    //kasowanie
    if (!intercut || intercutEnd<=timeline->GetEnd())
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
        //timeline->Grow(_timelines[i]->GetEnd());
        //edycja
        for (;i<(int)_timelines.size();i++)
        {
            _timelines[i]->NumberInLine=i;
            _timelines[i]->SetStartRel(_timelines[i-1]->GetEndRel());
        }
    }
    i = timeline->NumberInLine;
    if (intercut!=nullptr) AddTimeline(new Timeline(timeline->GetEnd(),intercutEnd,intercut->GetStep(),this));
    else if (i!=0 && timeline->GetStep()==_timelines[i-1]->GetStep())
    {
        if (i!=(int)_timelines.size()-1 && timeline->GetStep()==_timelines[i+1]->GetStep()) AddTimeline(new Timeline (_timelines[i-1]->GetStart(),_timelines[i+1]->GetEnd(),timeline->GetStep(),this));
        else AddTimeline(new Timeline (_timelines[i-1]->GetStart(),timeline->GetEnd(),timeline->GetStep(),this));
    }
    else if (i!=(int)_timelines.size()-1 && timeline->GetStep()==_timelines[i+1]->GetStep()) AddTimeline(new Timeline (timeline->GetStart(),_timelines[i+1]->GetEnd(),timeline->GetStep(),this));
    control--;
    if (!control) UpdateEventsReals();
    return 0;
}

int TimeMaster::AddEvent(Event* event, QString &info, bool coon)
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
    event->id = pos++;
    for (;pos<(int)_events.size();pos++) _events[pos]->id = pos;
    event->realPos = _timelines[timelinePos]->GetReal(event->GetDateStart());
    if (event->isDual) event->isDual = _timelines[endPos]->GetReal(event->GetDateEnd());
    if (coon)
    {
        connect(event,&Event::SignOut, this, &TimeMaster::OnSignOut);
        connect(event,&Event::DateChange, this, &TimeMaster::OnDateChange);
    }
    return 0;
}

int TimeMaster::SaveProject()
{
    QFile file(Filename);
    if (!file.open(QIODevice::WriteOnly| QIODevice::Truncate | QIODevice::Text)) return -1;
    QTextStream stream(&file);
    foreach(auto timeline, _timelines) timeline->Save(stream);
    foreach(auto event, _events) event->Save(stream);
    stream.flush();
    file.close();
    return 0;
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

void TimeMaster::UpdateEventsReals()
{
    int i = 0;
    foreach (auto event, _events)
    {
        while (!_timelines[i]->Contains(event->GetDateStart())) i++;
        event->realPos = _timelines[i]->GetReal(event->GetDateStart());
        if (event->isDual)
        {
            int j = i;
            while (!_timelines[j]->Contains(event->GetDateEnd())) j++;
            event->isDual = _timelines[j]->GetReal(event->GetDateEnd());
        }
    }
}

int TimeMaster::findRealPosition(unsigned step)
{
    int i = 0;
    foreach (auto timeline, _timelines)
    {
        if (step<=timeline->GetEndRel()) return i;
        ++i;
    }
    return i-1;
}

Timeline *TimeMaster::GetTimeline(int i)
{
    return _timelines[i];
}

std::vector<unsigned> TimeMaster::getTagOwners(QString tag)
{
    std::vector<unsigned> toRet = std::vector<unsigned>();
    auto IDs = tags->ProvideTagged(tag);
    foreach (auto id, IDs)
    {
        toRet.push_back(_events[*id]->TranslateId());
    }
    return toRet;
}

void TimeMaster::OnSignOut(int id)
{
    auto iterator = _events.begin();
    iterator+=id;
    _events.erase(iterator);
    for(;iterator!=_events.end();iterator++) (*iterator)->id--;

}

void TimeMaster::OnDateChange(unsigned id, bool* succes)
{
    Event* event = _events[id];
    (*succes)=false;
    if (_timelines.size()==0 ||
            _timelines[0]->GetStart()>event->GetDateStart() ||
            _timelines[TimelineCount()-1]->GetEnd()<event->GetDateStart() ||
            (event->isDual && _timelines[TimelineCount()-1]->GetEnd()<event->GetDateEnd())
            ) return;
    OnSignOut(id);
    QString s;
    AddEvent(event,s, 0);
    (*succes)=true;
}

void TimeMaster::OnNameChange(unsigned id, QString oldName, QString newName)
{
    if (newName.isEmpty()) _events[id]->ForgetTag(oldName);
    else _events[id]->RenameTag(oldName,newName);
}

unsigned TimeMaster::getLength()
{
    return _length;
}
