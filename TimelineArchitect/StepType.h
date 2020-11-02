#ifndef STEPTYPE_H
#define STEPTYPE_H
#include <QDate>


union step{
    QTime time;
    QDate date;
};

#endif // STEPTYPE_H
