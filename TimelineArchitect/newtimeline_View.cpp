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

    Timeline* timeline = new Timeline(ui->StartDate->date(),ui->EndDate->date(),(StepType)ui->Unit->currentIndex(),_master);
    if (timeline->GetStart().daysTo(timeline->GetEnd())>2 && timeline->GetStart()<timeline->GetEnd())
    {
    _master->AddTimeline(timeline);
    _master->updateLength();
    this->close();
    }
    else
    {
        delete timeline;
        QMessageBox msgBox;
        msgBox.setText("First date must be at least two days before second one");
        msgBox.exec();
    }
}
