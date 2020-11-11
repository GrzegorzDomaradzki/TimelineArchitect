#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QQuickItem>
#include <QWidget>
#include<event.h>
#include <StepType.h>
#include<memory>

class Timeline : public QObject
{
    Q_OBJECT

private:
    step _step;
    QDate _start;
    QDate _end;
    TimeMaster _boss;

public:
    explicit Timeline(QObject *parent = nullptr);

    bool Contains(QDate);
    int Distance(QDate);
    int Grow(QDate);
    int ShrinkLeft(QDate);
    int ShrinkRight(QDate);


    int GetStart();
    int GetEnd();

    int SetStep(step);
    step GetStep();

    void Save(QTextStream out);
    void SaveWitchEvents(QTextStream out);

};

#endif // TIMELINE_H
