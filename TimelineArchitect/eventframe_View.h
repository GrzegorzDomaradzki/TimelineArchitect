#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include <QFrame>
#include "event.h"

namespace Ui {
class EventFrame;
}

class EventFrame : public QFrame
{
    Q_OBJECT

public:
    explicit EventFrame(QWidget *parent = nullptr);
    EventFrame(unsigned new_id, QWidget *parent = nullptr);
    EventFrame(unsigned new_id,Event* event, QWidget *parent = nullptr);
    ~EventFrame();

    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

signals:
    void forgetMe(unsigned _id);

private:
    Ui::EventFrame *ui;
    Event* _event;
    QPoint _offset;
    unsigned _id;
};

#endif // EVENTFRAME_H
