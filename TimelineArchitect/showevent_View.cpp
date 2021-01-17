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
    auto start = event->GetDateStart();
    auto date = QString::number(start.day()) +"." + QString::number(start.month()) +"." +QString::number(abs(start.year()));
    if (start.year()<0) date+="BCE";
    if(event->IsBinary()) ui->date->setText(date);
    else
    {
        auto end = event->GetDateEnd();
        date += " <-> " +  QString::number(end.day()) +"." + QString::number(end.month()) +"." +QString::number(abs(end.year()));
        if (start.year()<0) date+="BCE";
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
