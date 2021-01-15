#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include <QFrame>
#include "event.h"
#include <QMouseEvent>
#include <QColor>

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
    Qt::GlobalColor GetColor();
    Qt::GlobalColor GetMarkedColor();
    void SetColor(Qt::GlobalColor);
    void SetMarkedColor(Qt::GlobalColor);
    void ChangeRadio();
    void SayGoodbye();



    unsigned* real;
    unsigned* dual;


    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

signals:
    void AddMe(unsigned int id, bool Add);
    void RewerseRepaint();


private slots:

    void on_checked_clicked(bool checked);

private:
    Ui::EventFrame *ui;
    Event* _event;
    QPoint _offset;
    unsigned _id;
    Qt::GlobalColor _color;
    Qt::GlobalColor _markedColor;

    void ChangeColors();
};

#endif // EVENTFRAME_H
