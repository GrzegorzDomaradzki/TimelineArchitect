#include "timeline.h"

Timeline::Timeline(QObject *parent) : QObject(parent)
{

}

Timeline::Timeline(QDate start, QDate end, StepType unit, QObject *parent): QObject(parent), _start(start), _end(end), _readPos(start),_startRel(0)
{
    _step = unit;
    UpdateLength();
}

bool Timeline::Contains(QDate date)
{
    if(_start<date && _end>date) return true;
    return false;
}

int Timeline::Distance(QDate date)
{
    if (Contains(date)) return 0;
    if (_start>date)  return -date.daysTo(_start);
    return _end.daysTo(date);

}

int Timeline::Grow(QDate date)
{
    int dist = Distance(date);
    if (dist==0) return 0;
    if(dist>0) _end=date;
    else _start=date;
    UpdateLength();
    return dist;
}

int Timeline::ShrinkLeft(QDate date)
{
    int to_ret = _start.daysTo(date);
    _start=date;
    UpdateLength();
    return to_ret;
}

int Timeline::ShrinkRight(QDate date)
{
    int to_ret = date.daysTo(_end);
    _end=date;
    UpdateLength();
    return to_ret;
}

unsigned Timeline::GetLength()
{
    return _length;
}

void Timeline::UpdateLength()
{
    switch (_step)
    {
    case day: _length=_start.daysTo(_end);
        _multi = 1;
        break;
    case week: _length=_start.daysTo(_end)/7;
        _multi = 7;
        break;
    case month: _length= _end.month() - _start.month()+(_end.year()-_start.year())*12;
        _multi = 1;
        break;
    case year: _length = _end.year()-_start.year();
        _multi = 1;
        break;
    case decade:_length = (_end.year()-_start.year())/10;
        _multi = 10;
        break;
    case century:_length = (_end.year()-_start.year())/100;
        _multi = 100;
        break;
    case millennium:_length = (_end.year()-_start.year())/1000;
        _multi = 1000;
        break;
    }
    if (_length==0)_length=1;
}

void Timeline::SetStartRel(unsigned x)
{
    _startRel = x;
}

unsigned Timeline::GetStartRel()
{
    return _startRel;
}

unsigned Timeline::GetEndRel()
{
    return _startRel+_length;
}

int Timeline::GetReal(QDate date)
{
    return _length*_start.daysTo(date)/_start.daysTo(_end);
}

int Timeline::StepsAchead(int position, int ahead, std::vector<QString> *_toWrite)
{
    QDate curr = _start;
    if(position!=-1)
    {
        position -=_startRel;
    }
    else position = 0;
    int rest = _length-position;
    if (ahead<rest) rest = ahead;

    switch (_step)
    {
    case day:
    case week: curr=curr.addDays(position*_multi);
        for (int i=0;i<rest;i+=15)
        {
            _toWrite->push_back(curr.toString("yyyy.MM.dd"));
            curr=curr.addDays(15*_multi);
        }
        break;
    case month: curr=curr.addMonths(position);
        for (int i=0;i<rest;i+=15)
        {
            _toWrite->push_back(curr.toString("yyyy.MM"));
            curr=curr.addMonths(15);
        }
        break;
    default: curr=curr.addYears(position*_multi);
        for (int i=0;i<rest;i+=15)
        {
            _toWrite->push_back(curr.toString("yyyy"));
            curr=curr.addYears(15*_multi);
        }
        break;
    }
    _toWrite->push_back(_end.toString("yyyy.MM.dd"));
    return rest;
}

std::vector<QString> Timeline::GetText(unsigned start, unsigned end)
{
    std::vector<QString> vector;
    if (start<_startRel) start=_startRel;
    if (end>_startRel+_length) end=_startRel+_length;
    start-=_startRel;
    end-=_startRel;
    return vector;
}



QDate Timeline::GetStart()
{
    return _start;
}

QDate Timeline::GetEnd()
{
    return _end;
}

int Timeline::SetStep(StepType unit)
{


    _step = unit;
    UpdateLength();
    return 1;
}

StepType Timeline::GetStep()
{
    return _step;
}

void Timeline::Save(QTextStream out)
{
    out<<"<Timeline>\n";
    out<< "\t<StartDate> " << _start.toString() <<" </StartDate>\n";
    out<< "\t<EndDate> " << _end.toString() <<" </EndDate>\n";

    out<< "\t<Step>" << UnitNames.at(_step) <<"</Step>\n";
    out<<"</Timeline>\n";

}


