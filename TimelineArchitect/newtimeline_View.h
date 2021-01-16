#ifndef NEWTIMELINE_H
#define NEWTIMELINE_H

#include <QDialog>
#include <timemaster.h>

namespace Ui {
class NewTimeline;
}

class NewTimeline : public QDialog
{
    Q_OBJECT

private:
     TimeMaster* _master;

public:
    explicit NewTimeline(QWidget *parent = nullptr);
    ~NewTimeline();

    void SetMaster(TimeMaster*);
    QDate GetDateStart(QString*);
    QDate GetDateEnd(QString*);
    void SetDateStart(QDate);
    void SetDateEnd(QDate);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewTimeline *ui;
};

#endif // NEWTIMELINE_H
