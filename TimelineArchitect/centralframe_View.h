#ifndef CENTRALFRAME_H
#define CENTRALFRAME_H

#include <QFrame>
#include "eventframe_View.h"
#include "event.h"
#include <vector>

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



private:
    unsigned _generator;
       std::map<unsigned,EventFrame*> _events_views;
        std::vector<unsigned> _toDelete;
        std::vector<unsigned> _selected;
    Ui::CentralFrame *ui;

};

#endif // CENTRALFRAME_H
