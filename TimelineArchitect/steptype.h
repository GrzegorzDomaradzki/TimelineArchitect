#ifndef STEPTYPE_H
#define STEPTYPE_H


#include <QDate>
#include <map>


enum Unit
{millennium, century, decade, year, month, day};

const static std::map<Unit,QString> UnitNames= {
    {millennium, "millennium"},
    {century,"century"},
    {decade,"decade"},
    {year,"year"},
    {month,"month"},
    {day,"day"} };


class StepType{
public:
    int jump;
    Unit unit;


    StepType(Unit New_Unit, int new_jump);
};
#endif // STEPTYPE_H
