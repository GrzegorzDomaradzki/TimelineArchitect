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
    //std::vector<unsigned> _events_id;
    //int _currID;
    unsigned _length;


public:
    Tags* tags;
    explicit TimeMaster(QObject *parent = nullptr);
    ~TimeMaster();

    int EventCount();
    int TimelineCount();


     int findPosition(QDate date);
     int findEventPosition(QDate date);

     int AddTimeline(Timeline*);


     int AddEvent(Event* event,QString& info, bool conn = 1);

     int SaveProject(QFile);
     int LoadProject(QFile);
     unsigned getLength();
     void updateLength();
     void UpdateEventsReals();

     int findRealPosition(unsigned);
     Timeline* GetTimeline(int);

     std::vector<unsigned> getTagOwners(QString tag);

signals:

public slots:
     void OnSignOut(int);
     void OnDateChange(unsigned id,bool* succes);

};

#endif // TIMEMASTER_H
