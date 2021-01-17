#include "eventframe_View.h"
#include "ui_eventframe_View.h"



EventFrame::EventFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{

    ui->setupUi(this);

}

EventFrame::EventFrame(unsigned new_id, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EventFrame)
{
    ui->setupUi(this);
    _id = new_id;
    real = dual = nullptr;
    _view = nullptr;

}

EventFrame::EventFrame(unsigned new_id, Event *event, QWidget *parent):
    QFrame(parent),
    ui(new Ui::EventFrame)
{
        this->setAutoFillBackground(true);
    connect(event,&Event::GetRelId,this,&EventFrame::OnGetRelId);
    ui->setupUi(this);
//    ui->DateLab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    ui->TitleLab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _color = event->color;
    _markedColor = Qt::red;
    _id = new_id;
    _event = event;
    real = &event->realPos;
    dual = &event->isDual;
    _view = nullptr;
    ResetData();
    this->raise();
}

EventFrame::~EventFrame()
{

    delete ui;
}

QColor EventFrame::GetColor()
{
    return _color;
}

QColor EventFrame::GetMarkedColor()
{
    return _markedColor;
}

void EventFrame::SetColor(QColor color)
{
    _event->color=_color = color;
    if (ui->checked->isChecked())
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, _color);
        this->setPalette(pal);
    }
}

void EventFrame::SetMarkedColor(QColor color)
{
    _markedColor = color;
    if (ui->checked->isChecked())
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, _markedColor);
        this->setPalette(pal);
    }
}

void EventFrame::ChangeRadio()
{
    ui->checked->setChecked(!ui->checked->isChecked());
    ChangeColors();
}

void EventFrame::SayGoodbye()
{
    _event->Unregister();
    deleteLater();
    this->close();
}

void EventFrame::ResetData()
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, _color);
    this->setPalette(pal);
    ui->TitleLab->setText(_event->name);
    ui->DescLab->setText(_event->text);
    auto start = _event->GetDateStart();
    QString date = QString::number(start.year()) +"." + QString::number(start.month()) +"." +QString::number(start.day());
    if(_event->IsBinary()) ui->DateLab->setText(date);
    else
    {
        auto end = _event->GetDateEnd();
        date += " <-> " +  QString::number(end.year()) +"." + QString::number(end.month()) +"." +QString::number(end.day());
        ui->DateLab->setText(date);
    }
    auto size = ui->TitleLab->sizeHint().width()+10;
    int altSize;
    if (size<(altSize= ui->DateLab->sizeHint().width()+ui->checked->sizeHint().width()+ ui->ShowButton->sizeHint().width())) size = altSize+10;
    this->resize(size,height());
}




void EventFrame::mousePressEvent(QMouseEvent *event)
{
    emit RewerseRepaint();
    _offset = event->pos();
    this->raise();
}


void EventFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - _offset));
    }
}

void EventFrame::mouseReleaseEvent(QMouseEvent *event)
{
    auto par = (QFrame*) parent();
    auto pos = x();
    if (pos< 0)
    {
        this->move(mapToParent(event->pos() - QPoint(pos,0) - _offset));
     }
    else
    {
        pos = pos+width() - par->width();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(pos,0) - _offset));
    }
    pos = y();
    if(pos<0)
    {
        this->move(mapToParent(event->pos() + QPoint(0,-pos))- _offset);
    }
    else
    {
        pos = pos+height() - par->height();
        if (pos>0)this->move(mapToParent(event->pos() - QPoint(0,pos) - _offset));
    }
    emit RewerseRepaint();
}

void EventFrame::mouseDoubleClickEvent(QMouseEvent*)
{

    NewEvent dialog(false,_event);
    dialog.SetMaster(nullptr);
    dialog.exec();
    ResetData();
}


void EventFrame::ChangeColors()
{
    QPalette pal = palette();
    if (ui->checked->isChecked()) pal.setColor(QPalette::Background, _markedColor);
    else pal.setColor(QPalette::Background, _color);
    this->setPalette(pal);
}

void EventFrame::on_checked_clicked(bool checked)
{
    emit AddMe(_id,checked);
    ChangeColors();
}

void EventFrame::OnGetRelId(unsigned *id)
{
    *id = _id;
}


void EventFrame::on_ShowButton_clicked()
{
    if (_view!= nullptr)
    {
        delete _view;
    }
    _view = new ShowEvent_View(_event);
    _view->show();
}
