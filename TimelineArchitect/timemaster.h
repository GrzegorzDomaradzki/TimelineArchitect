#ifndef TIMEMASTER_H
#define TIMEMASTER_H

#include <QObject>
#include <QDate>
#include <QFile>
#include<timeline.h>
#include<tags.h>

class TimeMaster : public QObject
{
    Q_OBJECT
private:

    std::vector<std::shared_ptr<Timeline>> _timelines;
    std::vector<std::shared_ptr<Event>> _events;
    std::map<unsigned,std::shared_ptr<Event>> _events_id;
    int _currID;


public:
    Tags* tags;
    explicit TimeMaster(QObject *parent = nullptr);


     int AskForDate(QDate,QTime/*optional*/);
     int AskForDate(QDate);

     int DropTimeline(int id);
     int CutTimeline(int id);
     int AddTimeline(Timeline);

     int CreateEvent();
     int AddEvent(Event event,QString& info);
     bool ShiftEvent(Event event,QString& info);

     int SaveProject(QFile);
     int LoadProject(QFile);
     int getNewId();

signals:

};

#endif // TIMEMASTER_H
