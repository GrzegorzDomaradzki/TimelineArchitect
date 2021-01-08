#ifndef CENTRALFRAME_H
#define CENTRALFRAME_H

#include <QFrame>
#include "eventframe_View.h"
#include "event.h"
#include <vector>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QPainterPath>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

namespace Ui {
class CentralFrame;
}

class CentralFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CentralFrame(QWidget *parent = nullptr);
    ~CentralFrame();
    void AddEvent(Event* event);
    int GetResolution();
    void SetResolution(int res);
    void UpdateTimelineData();

    void paintEvent(QPaintEvent *event) override;
    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void moveEvents(int diff);
    void PrintDate(unsigned,QPainter*);
    void PrintArrow(unsigned,QPainter*);


    Qt::GlobalColor color;

private:
    unsigned _generator;
    std::map<unsigned,EventFrame*> _eventsViews;
    std::vector<unsigned> _toDelete;
    std::vector<unsigned> _selected;
    Ui::CentralFrame *ui;
    unsigned int _relativePosition;
    QPoint _offset;
    int _resolution;
    int _end;

};

#endif // CENTRALFRAME_H
