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
    unsigned int id;
    step _step;



    std::vector<std::unique_ptr<Event>> _events;

public:
    explicit Timeline(QObject *parent = nullptr);

    void sortByDate();
    int AddEvent(Event);
    int SetStep(step);
    step GetStep();



signals:

};

#endif // TIMELINE_H
