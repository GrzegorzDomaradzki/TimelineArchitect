#include "timemaster.h"

TimeMaster::TimeMaster(QObject *parent) : QObject(parent)
{
    tags = new Tags();
}

TimeMaster::~TimeMaster()
{

    delete tags;
}

int TimeMaster::AddTimeline(Timeline)
{

}

int TimeMaster::AddEvent(Event &event, QString &info)
{

}

bool TimeMaster::ShiftEvent(Event &event, QString &info)
{

}
