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
#include <QMessageBox>

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
    void EraseSelected();

    void paintEvent(QPaintEvent *event) override;
    void  mousePressEvent(QMouseEvent *event) override;


    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;

    void moveEvents(int diff);
    void PrintDate(unsigned,QPainter*,int ascended = 0);
    void PrintArrow(unsigned,QPainter*);
    void PrintEventLines(QPainter*);
    int GetDist(int ahead, int position = -1);
    int Reload(int now, int ahead, QPainter* painter);
    void UnselectAll();
    void HideGiven(std::vector<unsigned> IDs);
    void ShowGiven(std::vector<unsigned> IDs);
    void ShowAll();
    void Purge();
    void SetSelectedColor(QColor);


    void AddHim(unsigned int id,bool add);

    QColor GetSelectedColor();
    QString GetNext();
    QColor color;


private:
    QMap<int,EventFrame*> _eventsViews;
    std::vector<QString> _toWrite;
    int _bookmark;
    int _generator;
    //std::vector<unsigned> _toDelete;
    std::vector<unsigned> _selected;
    Ui::CentralFrame *ui;
    unsigned int _relativePosition;
    QPoint _offset;
    int _resolution;
    int _end;
    bool _noPaint;
    QColor _selectedColor;

public slots:
    void OnRewersePaint();

    void OnAddMe(unsigned int id,bool add);
};

#endif // CENTRALFRAME_H
