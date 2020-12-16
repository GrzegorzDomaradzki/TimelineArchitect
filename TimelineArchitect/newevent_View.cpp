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

NewEvent::NewEvent(Event*& event, bool createMode, QWidget *parent):
    QDialog(parent),
    _event(event),
    _createMode(createMode),
    ui(new Ui::NewEvent)
{
    ui->setupUi(this);
    if (createMode)
    {
        //TODO: Set to edit
    }
}

void NewEvent::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewEvent::on_buttonBox_accepted()
{
    if(ui->EndDate->isEnabled()) _event = new Event(_master->tags,ui->StartDate->date(),ui->EndDate->date(),_master);
    else _event = new Event(_master->tags,ui->StartDate->date(),_master);
    QString info;
    if (_createMode)
    {
    if(_master->AddEvent(_event,info)==-1)
        {
            QMessageBox msgBox;
            msgBox.setText(info);
            msgBox.exec();
            delete _event;
            _event = nullptr;
            return;
        }
    }
    else
    {
        //TODO: Edit Event
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
