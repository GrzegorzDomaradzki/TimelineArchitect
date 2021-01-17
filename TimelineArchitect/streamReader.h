#ifndef STREAMREADER_H
#define STREAMREADER_H
#include "event.h"
#include "timeline.h"


class StreamReader
{
public:
     StreamReader();
     void ReadingLoop(QTextStream &stream, bool IgnoreTimelines,QObject* parent = nullptr);
     std::vector<Event *> EventList;
     std::vector<Timeline*> TimelineList;
     QStringList Tags;
     void Clear();
     QDate ValidDate(int y,int m, int d, bool &ok);

private:
     void ReadTimeline(QTextStream &stream);
     void ReadEvent(QTextStream &stream);
     void ReadTags(QTextStream &stream);
     QObject* _parent;
};

#endif // STREAMREADER_H
