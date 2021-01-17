#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QDialog>
#include "timemaster.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringListModel>
#include "textformatcontrol.h"

namespace Ui {
class NewEvent;
}

class NewEvent : public QDialog
{
    Q_OBJECT

private:
    TimeMaster* _master;
    Event* _event;
    QStringListModel* _tagList;
    bool _createMode;

public:
    explicit NewEvent(QWidget *parent = nullptr);
    ~NewEvent();
    NewEvent(bool createMode = true, Event* event = nullptr, QWidget *parent = nullptr);
    Event* GetEvent();
    QColor color;
    QDate GetDateStart(QString*);
    QDate GetDateEnd(QString*);
    void SetDateStart(QDate);
    void SetDateEnd(QDate);
    void InfoBox(QString info);
    void SetTags();
    void SetEnabled(bool state);

    void SetMaster(TimeMaster* master);

private slots:
    void on_buttonBox_accepted();
    void on_LongTime_stateChanged(int arg1);
    void on_pushButton_clicked();

    void on_buttonBox_rejected();

    void on_Add_clicked();

    void on_Remove_clicked();


private:
    Ui::NewEvent *ui;
    int Add(QDate Start, QDate End);
    int Modify(QDate Start, QDate End);
};

#endif // NEWEVENT_H
