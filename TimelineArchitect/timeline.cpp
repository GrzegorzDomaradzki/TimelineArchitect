#include "timeline.h"

Timeline::Timeline(QObject *parent) : QObject(parent)
{

}

Timeline::Timeline(QDate start, QDate end, Unit unit, int Jump, QObject *parent): QObject(parent), _start(start), _end(end), _readPos(start),_startRel(0)
{
    _step = std::make_shared<StepType>(unit,Jump);
    UpdateLength();
}

bool Timeline::Contains(QDate date)
{
    if(_start<date || _end>date) return true;
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
    int x = 0;
    switch (_step->unit)
    {
    case day: x=_start.daysTo(_end);
        break;
    case month: x= _end.month() - _start.month()+(_end.year()-_start.year())*12;
        break;
    case year: x = _end.year()-_start.year();
        break;
    case decade:x = _end.year()-_start.year()/10;
        break;
    case century:x = _end.year()-_start.year()/100;
        break;
    case millennium:x = _end.year()-_start.year()/1000;
        break;
    }
    _length = x/_step->jump;
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

int Timeline::SetStep(Unit unit, int Jump)
{


    _step->jump = Jump;
    _step->unit = unit;
    UpdateLength();
    return 1;
}

std::shared_ptr<StepType> Timeline::GetStep()
{
    return _step;
}

void Timeline::Save(QTextStream out)
{
    out<<"<Timeline>\n";
    out<< "\t<StartDate> " << _start.toString() <<" </StartDate>\n";
    out<< "\t<EndDate> " << _end.toString() <<" </EndDate>\n";

    out<< "\t<Step>" << UnitNames.at(_step->unit) << " " << _step->jump << "</Step>\n";
    out<<"</Timeline>\n";

}


