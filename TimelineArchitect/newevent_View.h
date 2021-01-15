#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QDialog>
#include "timemaster.h"
#include <QColorDialog>
#include <QMessageBox>

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
    NewEvent(bool createMode = true, Event* event = nullptr, QWidget *parent = nullptr);
    Event* GetEvent();
    QColor color;

    void SetMaster(TimeMaster* master);

private slots:
    void on_buttonBox_accepted();
    void on_LongTime_stateChanged(int arg1);
    void on_pushButton_clicked();

private:
    Ui::NewEvent *ui;
    int Add();
    int Modify();
};

#endif // NEWEVENT_H
