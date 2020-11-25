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

    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::EventFrame *ui;
    QPoint offset;
};

#endif // EVENTFRAME_H
