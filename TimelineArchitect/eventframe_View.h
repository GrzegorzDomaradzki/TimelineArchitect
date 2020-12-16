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
    EventFrame(unsigned new_id, QWidget *parent = nullptr);
    ~EventFrame();

    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

signals:
    void GiveMeStage(unsigned id);
    void forgetMe(unsigned id);

private:
    Ui::EventFrame *ui;
    QPoint offset;
    unsigned id;
};

#endif // EVENTFRAME_H
