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
        SetDateStart(_event->GetDateStart());
        if (event->isDual)
        {
            on_LongTime_stateChanged(1);
            ui->LongTime->setChecked(1);
            SetDateEnd(_event->GetDateEnd());
        }
        ui->Title->setText(_event->name);
        ui->textEdit->setText(_event->text);
        color = _event->color;
    }
    pal.setColor(QPalette::Background, color);
    ui->showColor->setPalette(pal);
}

Event *NewEvent::GetEvent()
{
    return _event;
}

QDate NewEvent::GetDateStart(QString* info)
{
    QDate date(ui->yearS->value(),ui->monthS->value(),1);
    *info = "";
    if (ui->yearS->value() == 0)
    {
        *info = "There is no year 0";
        return date;
    }
    if (date.daysInMonth()<=ui->dayS->value())
    {
        *info = "Month " + QString::number(date.month()) + " has only " + QString::number(date.daysInMonth()) + " days";
    }
    QDate date2(ui->yearS->value(),ui->monthS->value(),ui->dayS->value());
    return date2;
}

QDate NewEvent::GetDateEnd(QString* info)
{
    QDate date(ui->yearE->value(),ui->monthE->value(),1);
    *info = "";
    if (ui->yearE->value() == 0)
    {
        *info = "There is no year 0";
        return date;
    }
    if (date.daysInMonth()<=ui->dayE->value())
    {
        *info = "Month " + QString::number(date.month()) + " has only " + QString::number(date.daysInMonth()) + " days";
    }
    QDate date2(ui->yearE->value(),ui->monthE->value(),ui->dayE->value());
    return date2;
}

void NewEvent::SetDateStart(QDate date)
{
    ui->dayS->setValue(date.day());
    ui->monthS->setValue(date.month());
    ui->yearS->setValue(date.year());
}

void NewEvent::SetDateEnd(QDate date)
{
    ui->dayE->setValue(date.day());
    ui->monthE->setValue(date.month());
    ui->yearE->setValue(date.year());
}

void NewEvent::InfoBox(QString info)
{
    QMessageBox msgBox;
    msgBox.setText(info);
    msgBox.exec();
}

void NewEvent::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewEvent::on_buttonBox_accepted()
{

    QString info = "";
    QDate Start = GetDateStart(&info);
    if (!info.isEmpty())
    {
        InfoBox(info);
        return;
    }
    QDate End;
    if (ui->LongTime->isChecked())
    {
        End = GetDateEnd(&info);
        if (!info.isEmpty())
        {
            InfoBox(info);
            return;
        }
    }
    if (ui->Title->text().isEmpty())
    {
     InfoBox("Title can't be empty");
     return;
    }
    if (ui->LongTime->isChecked() && Start>=End)
    {
     InfoBox("Start date must be before end date");
     return;
    }
    if (_event==nullptr)
    {
        if (Add(Start, End)==-1) return;
    }
    else
    {
        if (Modify(Start, End)==-1) return;
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
        ui->dayE->setEnabled(1);
        ui->monthE->setEnabled(1);
        ui->yearE->setEnabled(1);
    }
    else
    {
        ui->LongTimeLabel->setEnabled(0);
        ui->dayE->setEnabled(0);
        ui->monthE->setEnabled(0);
        ui->yearE->setEnabled(0);
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

int NewEvent::Add(QDate Start, QDate End)
{
    if(ui->LongTime->isChecked())
    {
        _event = new Event(_master->tags,Start,End,_master);
    }
    else _event = new Event(_master->tags,Start,_master);
    QString info;
    if(_master->AddEvent(_event,info)==-1)
    {
        InfoBox(info);
        delete _event;
        _event = nullptr;
        return -1;
    }
    return 0;
}

int NewEvent::Modify(QDate Start, QDate End)
{
    int res = 0;
    auto oldDual = _event->isDual;
    if (ui->LongTime->isChecked() &&
            (_event->isDual==0 || _event->GetDateStart()!=Start || _event->GetDateEnd()!=End ))
    {
        _event->isDual = 1;
        res = _event->reincarnate(Start,End);
    }
    else if(_event->GetDateStart()!=Start ||
            (_event->isDual>0 && !ui->LongTime->isChecked()))
    {
          _event->isDual=0;
          res = _event->reincarnate(Start);
    }
    if (res==-1)
    {
        _event->isDual = oldDual;
        InfoBox("New dates outside timeline borders");
        return -1;
    }
    return 0;
}
