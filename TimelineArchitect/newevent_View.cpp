#include "newevent_View.h"
#include "ui_newevent.h"
#include <QDebug>
#include <QMessageBox>

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
    Event* event;
    if(ui->EndDate->isEnabled()) event = new Event(_master->tags,ui->StartDate->date(),ui->EndDate->date(),_master);
    else event = new Event(_master->tags,ui->StartDate->date(),_master);
    QString info;
    if(_master->AddEvent(event,info)==-1)
    {
        QMessageBox msgBox;
        msgBox.setText(info);
        msgBox.exec();
        return;
        delete event;
    }
    this->close();
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
