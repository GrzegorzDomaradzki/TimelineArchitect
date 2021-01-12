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

    std::vector<Timeline*> _timelines;
    std::vector<Event*> _events;
    std::vector<unsigned> _events_id;
    int _currID;
    unsigned _length;


public:
    Tags* tags;
    explicit TimeMaster(QObject *parent = nullptr);
    ~TimeMaster();

    int EventCount();
    int TimelineCount();



     int findPosition(QDate date);
     int findEventPosition(QDate date);

     int DropTimeline(int id);
     int CutTimeline(int id);
     int AddTimeline(Timeline*);

     int CreateEvent();
     int AddEvent(Event* event,QString& info);
     bool ShiftEvent(Event& event,QString& info);

     int SaveProject(QFile);
     int LoadProject(QFile);
     int getNewId();
     unsigned getLength();
     void updateLength();

     int findRealPosition(unsigned);
     Timeline* GetTimeline(int);

signals:

};

#endif // TIMEMASTER_H
