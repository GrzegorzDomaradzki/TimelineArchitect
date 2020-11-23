#include "newtimeline.h"
#include "ui_newtimeline.h"

NewTimeline::NewTimeline(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTimeline)
{
    ui->setupUi(this);
}

NewTimeline::~NewTimeline()
{
    delete ui;
}
