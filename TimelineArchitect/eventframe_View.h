#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include <QFrame>

namespace Ui {
class EventFrame;
}

class EventFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EventFrame(QWidget *parent = nullptr);
    ~EventFrame();

private:
    Ui::EventFrame *ui;
};

#endif // EVENTFRAME_H
