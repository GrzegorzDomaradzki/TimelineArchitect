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
#include <timemaster.h>

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
    TimeMaster *timeEngine;

    explicit CentralFrame(QWidget *parent = nullptr);
    ~CentralFrame();
    void AddEvent(Event* event);
    int GetResolution();
    void SetResolution(int res);
    void Redraw();
    void UpdateTimelineData();

    void paintEvent(QPaintEvent *event) override;
    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void moveEvents(int diff);
    void PrintDate(unsigned,QPainter*,int ascended = 0);
    void PrintArrow(unsigned,QPainter*);
    void PrintEventLines(QPainter*);
    int GetDist(int ahead, int position = -1);
    int Reload(int now, int ahead, QPainter* painter);
    QString GetNext();
    Qt::GlobalColor color;

private:

    std::vector<QString> _toWrite;
    int _bookmark;
    unsigned _generator;
    std::map<unsigned,EventFrame*> _eventsViews;
    std::vector<unsigned> _toDelete;
    std::vector<unsigned> _selected;
    Ui::CentralFrame *ui;
    unsigned int _relativePosition;
    QPoint _offset;
    int _resolution;
    int _end;

public slots:
    void OnStopDraw();
    void OnResumeDraw();
    void OnForgotten(int id);
};

#endif // CENTRALFRAME_H
