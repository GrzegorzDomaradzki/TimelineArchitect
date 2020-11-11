#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDate>
#include<QFile>
#include<QVector>
#include<timemaster.h>

class Event : public QObject
{
    Q_OBJECT
private:
        unsigned int _id;
        QDate _startDate;
        QDate _endDate;
        bool _isBinary;
        std::vector<QString> _ownedTags;
        TimeMaster* _boss;


public:
        QString text;
        QString multimedia;
        QString name;

        QVector<QString> ReadTags;
        bool AddTag(QString TagName,QString& info);
        bool RemoveTag(QString TagName);

        bool IsBinary();

        bool reincarnate(QDate,QString& info);
        bool reincarnate(QDate,QDate,QString& info);
        void Save(QTextStream out);


public:
    explicit Event(QObject *parent = nullptr);
    explicit Event(TimeMaster* boss, QDate start, unsigned int id, QObject *parent = nullptr);
    explicit Event(TimeMaster* boss, QDate start, unsigned int id, QDate end, QObject *parent = nullptr);
        ~Event();

signals:

};

#endif // EVENT_H
