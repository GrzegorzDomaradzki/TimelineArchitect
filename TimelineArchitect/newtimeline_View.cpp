#include "newtimeline_View.h"
#include "ui_newtimeline_View.h"
#include <QDebug>
#include <QMessageBox>

NewTimeline::NewTimeline(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTimeline)
{
    ui->setupUi(this);
    _master = nullptr;
}

NewTimeline::~NewTimeline()
{
    delete ui;
}

void NewTimeline::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewTimeline::on_buttonBox_accepted()
{
    QMessageBox msgBox;
    QString info;
    QDate Start = GetDateStart(&info);
    if (!info.isEmpty())
    {
        msgBox.setText(info);
        msgBox.exec();
        return;
    }
    QDate End = GetDateEnd(&info);
    if (!info.isEmpty())
    {
        msgBox.setText(info);
        msgBox.exec();
        return;
    }
    Timeline* timeline = new Timeline(Start,End,(StepType)ui->Unit->currentIndex(),_master);
    if (timeline->GetStart().daysTo(timeline->GetEnd())>2 && timeline->GetStart()<timeline->GetEnd())
    {
    _master->AddTimeline(timeline);
    this->close();
    }
    else
    {
        delete timeline;
        msgBox.setText("First date must be at least two days before second one");
        msgBox.exec();
    }
}

QDate NewTimeline::GetDateStart(QString* info)
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

QDate NewTimeline::GetDateEnd(QString* info)
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

void NewTimeline::SetDateStart(QDate date)
{
    ui->dayS->setValue(date.day());
    ui->monthS->setValue(date.month());
    ui->yearS->setValue(date.year());
}

void NewTimeline::SetDateEnd(QDate date)
{
    ui->dayE->setValue(date.day());
    ui->monthE->setValue(date.month());
    ui->yearE->setValue(date.year());
}
