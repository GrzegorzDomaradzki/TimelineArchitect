#ifndef TIMEMASTER_H
#define TIMEMASTER_H

#include <QObject>
#include <QDate>
#include <QFile>
#include<timeline.h>

class TimeMaster : public QObject
{
    Q_OBJECT
private:
    std::map<std::string,int> _tagsNames;
    std::vector<std::vector<int>> _tagOwners;
    std::vector<Timeline*> _timelines;



public:
    explicit TimeMaster(QObject *parent = nullptr);
     bool RegisterTagOwner(QString,unsigned);
     bool RegisterTag(QString);
     int RenameTag(QString);
     int AskForDate(QDate,QTime/*optional*/);

     int DropTimeline(int id);
     int CutTimeline(int id);
     int AddTimeline(Timeline);

     int SaveProject(QFile);
     int LoadProject(QFile);


signals:

};

#endif // TIMEMASTER_H
