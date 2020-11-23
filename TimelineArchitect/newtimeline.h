#ifndef NEWTIMELINE_H
#define NEWTIMELINE_H

#include <QDialog>

namespace Ui {
class NewTimeline;
}

class NewTimeline : public QDialog
{
    Q_OBJECT

public:
    explicit NewTimeline(QWidget *parent = nullptr);
    ~NewTimeline();

private:
    Ui::NewTimeline *ui;
};

#endif // NEWTIMELINE_H
