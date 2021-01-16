#ifndef SHOWEVENT_VIEW_H
#define SHOWEVENT_VIEW_H

#include <QDialog>
#include <QPalette>
#include <QStringListModel>
#include "event.h"

namespace Ui {
class ShowEvent_View;
}

class ShowEvent_View : public QDialog
{
    Q_OBJECT

public:
    explicit ShowEvent_View(QWidget *parent = nullptr);
    ShowEvent_View(Event* event,QWidget *parent = nullptr);
    ~ShowEvent_View();

private:
    Ui::ShowEvent_View *ui;
    QStringListModel* _tagList;
};

#endif // SHOWEVENT_VIEW_H
