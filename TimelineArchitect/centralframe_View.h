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


public slots:
    void OnStageRequest(unsigned id);



private:
    unsigned generator;
       std::map<unsigned,EventFrame*> events_views;
        std::vector<unsigned> toDelete;
        std::vector<unsigned> selected;

    Ui::CentralFrame *ui;

};

#endif // CENTRALFRAME_H
