#ifndef STEPTYPE_H
#define STEPTYPE_H


#include <QDate>
#include <map>


enum StepType
{millennium, century, decade, year, month, week, day};

const static std::map<StepType,QString> UnitNames= {
    {millennium, "millennium"},
    {century,"century"},
    {decade,"decade"},
    {year,"year"},
    {month,"month"},
    {week,"month"},
    {day,"day"} };


#endif // STEPTYPE_H
