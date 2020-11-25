#ifndef CENTRALFRAME_H
#define CENTRALFRAME_H

#include <QFrame>
#include "eventframe_View.h"
#include "event.h"
#include <vector>

namespace Ui {
class CentralFrame;
}

class CentralFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CentralFrame(QWidget *parent = nullptr);
    ~CentralFrame();






private:
       std::vector<EventFrame*> events_views;
        std::vector<unsigned> toDelete;
        std::vector<unsigned> selected;

    Ui::CentralFrame *ui;

};

#endif // CENTRALFRAME_H
