#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>

#include <steptype.h>
#include <memory>
#include <QTextStream>
#include <event.h>


class Timeline : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<StepType> _step;
    QDate _start;
    QDate _end;
    QDate _readPos;
    unsigned _length;

    unsigned _startRel;

public:
    unsigned NumberInLine;

    explicit Timeline(QObject *parent = nullptr);
    explicit Timeline(QDate start,QDate end,Unit unit, int Jump,QObject *parent = nullptr);

    bool Contains(QDate);
    int Distance(QDate);
    int Grow(QDate);
    int ShrinkLeft(QDate);
    int ShrinkRight(QDate);

    unsigned GetLength();
    void UpdateLength();
    void SetStartRel(unsigned x);
    unsigned GetStartRel();
    unsigned GetEndRel();


    std::vector<QString> GetText(unsigned start, unsigned end); //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

    QDate GetStart();


    QDate GetEnd();


    int SetStep(Unit unit, int Jump);
    std::shared_ptr<StepType> GetStep();

    void Save(QTextStream out);


};

#endif // TIMELINE_H
