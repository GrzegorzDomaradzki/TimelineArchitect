#include "newevent.h"
#include "ui_newevent.h"
#include <QDebug>

NewEvent::NewEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEvent)
{
    ui->setupUi(this);
}

NewEvent::~NewEvent()
{
    delete ui;
}

void NewEvent::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewEvent::on_buttonBox_accepted()
{
 qDebug() << "accept";
}

void NewEvent::on_LongTime_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->LongTimeLabel->setEnabled(1);
        ui->EndDate->setEnabled(1);
    }
    else
    {
        ui->LongTimeLabel->setEnabled(0);
        ui->EndDate->setEnabled(0);
    }
}
