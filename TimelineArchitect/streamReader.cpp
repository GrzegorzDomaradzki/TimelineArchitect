#include "streamReader.h"



StreamReader::StreamReader()
{
    EventList.clear();
    TimelineList.clear();
}

void StreamReader::ReadingLoop(QTextStream &stream, bool IgnoreTimelines, QObject* parent)
{
    _parent = parent;
    QString word = "";
    for (stream>>word;!word.isEmpty();stream>>word)
    {
        if (word[0]!="<") continue;
        if (word.compare("<Event>")==0)
        {
            ReadEvent(stream);
            continue;
        }
        if (word.compare("<Timeline>")==0)
        {
            if(!IgnoreTimelines)ReadTimeline(stream);
            else do stream >> word; while(!word.isEmpty() && word.compare("</Timeline>"));
            continue;
        }
        Clear();
        throw "error in savefile - unknown tag: " + word;
    }

}

void StreamReader::Clear()
{
    foreach (auto evet, EventList) delete evet;
    foreach (auto timeline, TimelineList) delete timeline;
    EventList.clear();
    TimelineList.clear();
}

QDate StreamReader::ValidDate(int y,int m, int d, bool &ok)
{
    ok = false;
    QDate date(y,m,1);
    if (date.daysInMonth()<d) return date;
    QDate date2(y,m,d);
    ok = true;
    return date2;
}

void StreamReader::ReadTimeline(QTextStream &stream)
{
    bool a,b,c;
    a=b=c=false;
    QDate StartDate;
    QDate EndDate;
    StepType step;
    QString word = "";
    for (stream>>word;!word.isEmpty();stream>>word)
    {
        if (word[0]!="<") continue;
        if (word.compare("<StartDate>")==0)
        {
            a=true;
            stream >> word;
            int d,m,y;
            bool ok;
            sscanf(word.toStdString().c_str(),"%i.%i.%i",&d,&m,&y);
            StartDate = ValidDate(y,m,d,ok);
            if (!ok)
            {
                Clear();
                throw "date " + word + " is not valid";
            }
            do stream >> word;
            while(!word.isEmpty() && word.compare("</StartDate>"));
            continue;
        }
        if (word.compare("<EndDate>")==0)
        {
            b=true;
            stream >> word;
            int d,m,y;
            bool ok;
            sscanf(word.toStdString().c_str(),"%i.%i.%i",&d,&m,&y);
            EndDate = ValidDate(y,m,d,ok);
            if (!ok)
            {
                Clear();
                throw "date " + word + " is not valid";
            }
            do stream >> word; while(!word.isEmpty() && word.compare("</EndDate>"));
            continue;
        }
        if (word.compare("<Step>")==0)
        {
            c=true;
            stream >> word;
            if (NamesUnit.find(word)!=NamesUnit.end()) step = NamesUnit.at(word);
            else
            {
               Clear();
               throw "unknown StepType: " + word;
            }
            do stream >> word; while(!word.isEmpty() && word.compare("</Step>"));
            continue;
        }
        if (word.compare("</Timeline>")==0) break;
        Clear();
        throw "error in savefile - unknown tag: " + word;
    }
    if (word.isEmpty())
    {
        QString s = QString::number(TimelineList.size()+1);
        Clear();
        throw  "Ending tags in timeline " + s + " not matched";
    }
    if (a==false || b==false || c==false)
    {
        QString s = QString::number(TimelineList.size()+1);
        Clear();
        throw "Incomplete Timeline number " + s +" Timeline shoud have tags <Step>, <StartDate>, <EndDate>";
    }
    if (StartDate>=EndDate)
    {
        QString s = QString::number(TimelineList.size()+1);
        Clear();
        throw "Timeline " + s + " has 0 or negative length";
    }
    TimelineList.push_back(new Timeline(StartDate, EndDate, step,_parent));
}


void StreamReader::ReadEvent(QTextStream &stream)
{
    bool a,b;
    a=b=false;
    QDate StartDate;
    QDate EndDate;
    unsigned isDual =0;
    QColor color = Qt::lightGray;
    QString title = "";
    QString text ="";
    QString word = "";
    Tags.clear();
    for (stream>>word;!word.isEmpty();stream>>word)
    {
        if (word[0]!="<") continue;
        if (word.compare("<StartDate>")==0)
        {
            a=true;
            stream >> word;
            int d,m,y;
            bool ok;
            sscanf(word.toStdString().c_str(),"%i.%i.%i",&d,&m,&y);
            StartDate = ValidDate(y,m,d,ok);
            if (!ok)
            {
                Clear();
                throw "date " + word + " is not valid";
            }
            do stream >> word; while(!word.isEmpty() && word.compare("</StartDate>"));
            continue;
        }
        if (word.compare("<EndDate>")==0)
        {
            isDual=true;
            stream >> word;
            int d,m,y;
            bool ok;
            sscanf(word.toStdString().c_str(),"%i.%i.%i",&d,&m,&y);
            EndDate = ValidDate(y,m,d,ok);
            if (!ok)
            {
                Clear();
                throw "date " + word + " is not valid";
            }
            do stream >> word; while(!word.isEmpty() && word.compare("</EndDate>"));
            continue;
        }
        if (word.compare("<Title>")==0)
        {
            b = true;
            stream >> word;
            while(!word.isEmpty() && word.compare("</Title>"))
            {
                title+=word+" ";
                stream >> word;
            }
            continue;
        }
        if (word.compare("<Color>")==0)
        {
            stream>> word;
            color.setNamedColor(word);
            do stream >> word; while(!word.isEmpty() && word.compare("</Color>"));
            continue;
        }
        if (word.compare("<Text>")==0)
        {
            stream >> word;
            while(!word.isEmpty() && word.compare("</Text>"))
            {
                text+=word+" ";
                stream >> word;
            }
            continue;
        }
        if (word.compare("<Tags>")==0)
        {
            ReadTags(stream);
            continue;
        }
        if (word.compare("</Event>")==0) break;
        Clear();
        throw "error in savefile - unknown tag: " + word;
    }
    if (word.isEmpty())
    {
        QString s = QString::number(EventList.size()+1);
        Clear();
        throw  "Ending tags in Event number " + s + " not matched";
    }
    if (a==false || b==false )
    {
        QString s = QString::number(EventList.size()+1);
        Clear();
        throw "Incomplete Event number " + s +" Timeline shoud have tags <StartDate>, <Title>";
    }
    if (isDual && StartDate>=EndDate)
    {
        QString s = QString::number(EventList.size()+1);
        Clear();
        throw "Event " + s + " has 0 or negative length";
    }
    Event* event;
    if (isDual) EventList.push_back(event = new Event(nullptr,StartDate,EndDate,_parent));
    else EventList.push_back(event = new Event(nullptr,StartDate,_parent));
    QString ignore;
    foreach (auto tag, Tags) event->AddTag(tag,ignore);
    event->text = text;
    event->name = title;
    event->color = color;
}

void StreamReader::ReadTags(QTextStream &stream)
{
    QString word;
    QString ignore;
    for (stream>>word;!word.isEmpty();stream>>word)
    {
        if (word[0]!="<") continue;
        if (word.compare("<Tag>")==0)
        {
            stream>>word;
            Tags.push_back(word);
            stream>>word;
            if (word.compare("</Tag>")==0) continue;
            QString s = QString::number(EventList.size()+1);
            Clear();
            throw  "Ending Tags tags in Event number " + s + " not matched";
        }
        if (word.compare("</Tags>")==0) break;
        Clear();
        throw "error in savefile - unknown tag: " + word;
    }
    if (word.isEmpty())
    {
        QString s = QString::number(EventList.size()+1);
        Clear();
        throw  "Ending Tags tags in Event number " + s + " not matched";
    }
}
