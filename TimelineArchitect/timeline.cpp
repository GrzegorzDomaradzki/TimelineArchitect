#include "timeline.h"

Timeline::Timeline(QObject *parent) : QObject(parent)
{

}

Timeline::Timeline(QDate start, QDate end, Unit unit, int Jump, QObject *parent): QObject(parent), _start(start), _end(end)
{
    _step = std::make_shared<StepType>(unit,Jump);
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
    return dist;
}

int Timeline::ShrinkLeft(QDate date)
{
    int to_ret = _start.daysTo(date);
    _start=date;
    return to_ret;
}

int Timeline::ShrinkRight(QDate date)
{
    int to_ret = date.daysTo(_end);
    _end=date;
    return to_ret;
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


