#ifndef TIMEMASTER_H
#define TIMEMASTER_H

#include <QObject>
#include <tags.h>
#include <timeline.h>
#include <event.h>


#include <QObject>
#include <QDate>
#include <QFile>
#include <memory>


class TimeMaster : public QObject
{
    Q_OBJECT
private:

    std::vector<std::shared_ptr<Timeline>> _timelines;
    std::vector<std::shared_ptr<Event>> _events;
    std::vector<unsigned> _events_id;
    int _currID;




public:
    Tags* tags;
    explicit TimeMaster(QObject *parent = nullptr);
    ~TimeMaster();

    int EventCount();
    int TimelineCount();


     int AskForDate(QDate,QDate);
     int AskForDate(QDate);

     int findPosition(int start, int end, QDate date);
     int findEventPosition(int start, int end, QDate date);

     int DropTimeline(int id);
     int CutTimeline(int id);
     int AddTimeline(Timeline*,int& succes,QString &info);

     int CreateEvent();
     int AddEvent(Event* event,QString& info);
     bool ShiftEvent(Event& event,QString& info);

     int SaveProject(QFile);
     int LoadProject(QFile);
     int getNewId();

signals:

};

#endif // TIMEMASTER_H
