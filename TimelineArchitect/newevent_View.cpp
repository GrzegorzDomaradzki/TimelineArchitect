#include "newevent_View.h"
#include "ui_newevent_View.h"
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

NewEvent::NewEvent(bool createMode, Event* event, QWidget *parent):
    QDialog(parent),
    _event(nullptr),
    _createMode(createMode),
    ui(new Ui::NewEvent)
{

    ui->setupUi(this);
    ui->showColor->setAutoFillBackground(1);
    QPalette pal = ui->showColor->palette();
    color = Qt::lightGray;
    if (createMode==false)
    {
        _event = event;
        color = _event->color;
        ui->StartDate->setDate(_event->GetDateStart());
        if (event->isDual)
        {
            on_LongTime_stateChanged(1);
            ui->EndDate->setDate(_event->GetDateEnd());
        }
        ui->Title->setText(_event->name);
        ui->textEdit->setText(_event->text);
    }
    pal.setColor(QPalette::Background, color);
    ui->showColor->setPalette(pal);
}

Event *NewEvent::GetEvent()
{
    return _event;
}

void NewEvent::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewEvent::on_buttonBox_accepted()
{
    if (ui->LongTime->isChecked() && ui->StartDate->date()>=ui->EndDate->date())
    {
     QMessageBox msgBox;
     msgBox.setText("Start date must be before end date");
     msgBox.exec();
     return;
    }
    if (_event==nullptr)
    {
        if (Add()==-1) return;
    }
    else
    {
        if (Modify()==-1) return;
    }
    _event->name=ui->Title->text();
    _event->text=ui->textEdit->toPlainText();
    _event->color = color;
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

void NewEvent::on_pushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(color,this);
    if (newColor.isValid())
    {
        color = newColor;
        QPalette pal = ui->showColor->palette();
        pal.setColor(QPalette::Background, color);
        ui->showColor->setPalette(pal);
    }
}

int NewEvent::Add()
{
    QMessageBox msgBox;
    if(ui->LongTime->isChecked())
    {
        _event = new Event(_master->tags,ui->StartDate->date(),ui->EndDate->date(),_master);
    }
    else _event = new Event(_master->tags,ui->StartDate->date(),_master);
    QString info;
    if(_master->AddEvent(_event,info)==-1)
    {
        msgBox.setText(info);
        msgBox.exec();
        delete _event;
        _event = nullptr;
        return -1;
    }
    return 0;
}

int NewEvent::Modify( )
{
    QMessageBox msgBox;
    int res = 0;
    auto oldDual = _event->isDual;
    if (ui->LongTime->isChecked() &&
            (_event->isDual==0 || _event->GetDateStart()!=ui->StartDate->date() || _event->GetDateEnd()!=ui->EndDate->date() ))
    {
        _event->isDual = 1;
        res = _event->reincarnate(ui->StartDate->date(),ui->EndDate->date());
    }
    else if(_event->GetDateStart()!=ui->StartDate->date() ||
            (_event->isDual>0 && !ui->LongTime->isChecked()))
    {
          _event->isDual=0;
          res = _event->reincarnate(ui->StartDate->date());
    }
    if (res==-1)
    {
        _event->isDual = oldDual;
        msgBox.setText("New dates outside timeline borders");
        msgBox.exec();
        return -1;
    }
    return 0;
}
