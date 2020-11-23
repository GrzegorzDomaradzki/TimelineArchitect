#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDate>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <tags.h>


class Event : public QObject
{
    Q_OBJECT
private:
        unsigned int _id;
        QDate _startDate;
        QDate _endDate;
        bool _isBinary;
        std::vector<QString> _ownedTags;
        Tags* _Tags;


public:
        QString text;
        QString multimedia;
        QString name;

        QVector<QString> ReadTags;
        bool AddTag(QString TagName,QString& info) ;
        bool RemoveTag(QString TagName) ;
        bool IsBinary();

        QDate GetDateStart() ;
        QDate GetDateEnd() ;

        bool reincarnate(QDate,QString& info);
        bool reincarnate(QDate,QDate,QString& info);
        void Save(QTextStream out);


public:
    explicit Event(QObject *parent = nullptr);
    explicit Event(Tags* boss, QDate start, unsigned int id, QObject *parent = nullptr);
    explicit Event(Tags* boss, QDate start, unsigned int id, QDate end, QObject *parent = nullptr);
        ~Event() override;

public slots:

signals:
    void OnDateChange(unsigned &id);

};

#endif // EVENT_H
