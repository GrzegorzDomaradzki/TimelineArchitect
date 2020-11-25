#include "newtimeline_View.h"
#include "ui_newtimeline.h"
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

    Timeline* timeline = new Timeline(ui->StartDate->date(),ui->EndDate->date(),static_cast<Unit>(ui->Unit->currentIndex()),ui->Step->value(),_master);
    QString info;
    int OK;
    _master->AddTimeline(timeline,OK,info);
    if (OK==-1)
    {
        QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Colision", info,
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
          // TODO
          }
          else
          {
              delete timeline;
              return;
          }
    }
    this->close();
}
