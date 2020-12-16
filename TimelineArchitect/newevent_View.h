#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QDialog>
#include "timemaster.h"

namespace Ui {
class NewEvent;
}

class NewEvent : public QDialog
{
    Q_OBJECT

private:
    TimeMaster* _master;
    Event* _event;
    bool _createMode;

public:
    explicit NewEvent(QWidget *parent = nullptr);
    ~NewEvent();
    NewEvent(Event*& event, bool createMode = true, QWidget *parent = nullptr);

    void SetMaster(TimeMaster* master);

private slots:
    void on_buttonBox_accepted();

    void on_LongTime_stateChanged(int arg1);


private:
    Ui::NewEvent *ui;
};

#endif // NEWEVENT_H
