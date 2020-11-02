#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDate>
#include<QFile>
#include<QVector>

class Event : public QObject
{
    Q_OBJECT
private:
        unsigned int id;
        QDate _startDate;
        QDate _endDate;
        bool _isBinary;
        std::map<int,QString> OwnedTags;

        int TryRegisterTag(QString);

public:
        QString text;
        QString multimedia;
        QString name;

        QVector<QString> ReadTags;
        bool AddTag(QString);
        bool RemoveTag(int);

        Event reincarnate(QDate);
        Event reincarnate(QDate,QDate);
        void Save(QFile);


public:
    explicit Event(QObject *parent = nullptr);

signals:

};

#endif // EVENT_H
