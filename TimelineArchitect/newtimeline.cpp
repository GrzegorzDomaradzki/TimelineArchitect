#include "newtimeline.h"
#include "ui_newtimeline.h"
#include <QDebug>

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
    qDebug() << "!!!";
    this->close();
}
