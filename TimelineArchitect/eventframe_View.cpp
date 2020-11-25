#include "eventframe_View.h"
#include "ui_eventframe.h"

EventFrame::EventFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{
    ui->setupUi(this);
}

EventFrame::~EventFrame()
{
    delete ui;
}
