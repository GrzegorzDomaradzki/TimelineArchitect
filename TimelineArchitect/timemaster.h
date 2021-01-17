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
#include <QTextStream>


class TimeMaster : public QObject
{
    Q_OBJECT
private:

    std::vector<Timeline*> _timelines;
    std::vector<Event*> _events;

    unsigned _length;


public:
    Tags* tags;
    QString Filename;
    explicit TimeMaster(QObject *parent = nullptr);
    ~TimeMaster();

    int EventCount();
    int TimelineCount();


     int findPosition(QDate date);
     int findEventPosition(QDate date);

     int AddTimeline(Timeline*);


     int AddEvent(Event* event,QString& info, bool conn = 1);

     int SaveProject();
     int LoadProject();
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
     void OnNameChange(unsigned, QString, QString);

};

#endif // TIMEMASTER_H
