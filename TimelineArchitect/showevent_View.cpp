#include "showevent_View.h"
#include "ui_showevent_View.h"

ShowEvent_View::ShowEvent_View(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowEvent_View)
{
    ui->setupUi(this);
}

ShowEvent_View::ShowEvent_View(Event *event, QWidget *parent) :
QDialog(parent),
ui(new Ui::ShowEvent_View)
{
    ui->setupUi(this);
    this->setWindowTitle(event->name);
    QPalette pal = ui->colorLabel1->palette();
    pal.setColor(QPalette::Background, event->color);
    ui->colorLabel1->setPalette(pal);
    pal = ui->colorLabel2->palette();
    pal.setColor(QPalette::Background, event->color);
    ui->colorLabel2->setPalette(pal);
    ui->title->setText(event->name);
    ui->textEdit->setText(event->text);
    if(event->IsBinary()) ui->date->setText(event->GetDateStart().toString("yyyy.MM.dd"));
    else
    {
        QString date = event->GetDateStart().toString("yyyy.MM.dd");
        date += " - " + event->GetDateEnd().toString("yyyy.MM.dd");
        ui->date->setText(date);
    }
    ui->listView->setModel(_tagList = new QStringListModel(this));
    auto tags = event->GetTags();
    _tagList->setStringList(tags);
}

ShowEvent_View::~ShowEvent_View()
{
    delete ui;
}
