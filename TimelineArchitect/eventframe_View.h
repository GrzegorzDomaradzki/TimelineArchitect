#ifndef EVENTFRAME_H
#define EVENTFRAME_H

#include <QFrame>
#include "event.h"
#include <QMouseEvent>
#include <QColor>
#include "newevent_View.h"
#include "showevent_View.h"

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
    QColor GetColor();
    QColor GetMarkedColor();
    void SetColor(QColor);
    void SetMarkedColor(QColor);
    void ChangeRadio();
    void SayGoodbye();
    void ResetData();



    unsigned* real;
    unsigned* dual;


    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent * event) override;

public slots:

signals:
    void AddMe(unsigned int id, bool Add);
    void RewerseRepaint();


private slots:

    void on_checked_clicked(bool checked);
    void OnGetRelId(unsigned* id);


    void on_ShowButton_clicked();

private:
    Ui::EventFrame *ui;
    Event* _event;
    QPoint _offset;
    unsigned _id;
    QColor _color;
    QColor _markedColor;
    ShowEvent_View* _view;

    void ChangeColors();
};

#endif // EVENTFRAME_H
