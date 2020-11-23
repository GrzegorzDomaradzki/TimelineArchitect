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


private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewTimeline *ui;
};

#endif // NEWTIMELINE_H
