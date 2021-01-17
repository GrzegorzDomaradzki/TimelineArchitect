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
    _master(nullptr),
    _event(nullptr),
    _createMode(createMode),
    ui(new Ui::NewEvent)
{


    ui->setupUi(this);
    ui->listView->setModel(_tagList = new QStringListModel(this));
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
        auto tags = _event->GetTags();
        _tagList->setStringList(tags);
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
    if (date.daysInMonth()<ui->dayS->value())
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
    if (date.daysInMonth()<ui->dayE->value())
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

void NewEvent::SetTags()
{
    auto current = _event->GetTags();
    QStringList toRemove;
    auto listed = _tagList->stringList();
    foreach (auto tag, current)
    {
      if (listed.removeOne(tag)) continue;
      toRemove.push_back(tag);
    }
    QString ignore;
    foreach (auto add, listed) _event->AddTag(add,ignore);
    foreach (auto remove, toRemove) _event->RemoveTag(remove);

}

void NewEvent::SetEnabled(bool state)
{
    ui->LongTimeLabel->setEnabled(state);
    ui->dayE->setEnabled(state);
    ui->monthE->setEnabled(state);
    ui->yearE->setEnabled(state);
    ui->label_acitve1->setEnabled(state);
    ui->label_acitve2->setEnabled(state);
    ui->label_acitve3->setEnabled(state);
}

void NewEvent::SetMaster(TimeMaster *master)
{
    _master=master;
}

void NewEvent::on_buttonBox_accepted()
{

    QString info = "";
    if (!TextFormatControl::TitleTextName(ui->Title->text(),info) || !TextFormatControl::TitleTextName(ui->textEdit->toPlainText(),info))
    {
        InfoBox(info);
        return;
    }
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
    SetTags();
    this->close();
}

void NewEvent::on_LongTime_stateChanged(int arg1)
{
    SetEnabled(arg1!=0);
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

void NewEvent::on_buttonBox_rejected()
{
    this->deleteLater();
}

void NewEvent::on_Add_clicked()
{
    QStringList list;
    if (_event==nullptr) list = _master->tags->ListActiveTag();
    else
    {
        list = _event->AllTags();
    }
    if (list.empty())
    {
        InfoBox("First create tag!");
        return;
    }
    foreach (auto string, _tagList->stringList()) list.removeOne(string);
    bool ok;
    QString toRet = QInputDialog::getItem(this,"Add Tag to event","Choose tag to add:", list, 0 ,0,&ok);
    if (!ok) return;
    if (!toRet.isEmpty())
    {
        _tagList->insertRow(0);
        auto index = _tagList->index(0);
        _tagList->setData(index,toRet);
        _tagList->sort(0);
        return;
    }
}

void NewEvent::on_Remove_clicked()
{
    auto row = ui->listView->currentIndex().row();
    _tagList->removeRow(row);
}


