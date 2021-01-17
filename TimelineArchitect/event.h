#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDate>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <tags.h>
#include <QColor>


class Event : public QObject
{
    Q_OBJECT
private:

        QDate _startDate;
        QDate _endDate;

        QStringList _ownedTags;
        Tags* _Tags;
        int _x,_y;

public:
        QString text;
        QColor color;
        QString name;
        unsigned int id;
        unsigned realPos;
        unsigned isDual;

        bool AddTag(QString TagName,QString& info) ;
        QStringList GetTags();
        bool RemoveTag(QString TagName) ;
        bool IsBinary();
        bool ForgetTag(QString tag);
        bool RenameTag(QString oldName,QString newName);

        QDate GetDateStart() ;
        QDate GetDateEnd() ;
        QStringList AllTags();

        int reincarnate(QDate);
        int reincarnate(QDate,QDate);
        void Save(QTextStream &out);
        void Unregister();
        unsigned TranslateId();

public:
    explicit Event(QObject *parent = nullptr);
    explicit Event(Tags* boss, QDate start,  QObject *parent = nullptr);
    explicit Event(Tags* boss, QDate start,  QDate end, QObject *parent = nullptr);
        ~Event() override;

public slots:

signals:
    void DateChange(unsigned id,bool* succes);
    void SignOut(unsigned id);
    void GetRelId(unsigned* id);

};

#endif // EVENT_H
