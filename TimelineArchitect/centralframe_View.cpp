#include "centralframe_View.h"
#include "ui_centralframe.h"



CentralFrame::CentralFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CentralFrame)
{
    ui->setupUi(this);
    _generator = 0;

    auto TestFrame1 =new EventFrame(_generator++, this);
    TestFrame1->setFrameShape(QFrame::StyledPanel);
    TestFrame1->setFrameShadow(QFrame::Raised);
    _events_views.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame1));

    auto TestFrame2 = new EventFrame(_generator++,this);
    TestFrame2->setFrameShape(QFrame::StyledPanel);
    TestFrame2->setFrameShadow(QFrame::Raised);
    _events_views.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame2));

    auto TestFrame3 =new EventFrame(_generator++,this);
    TestFrame3->setFrameShape(QFrame::StyledPanel);
    TestFrame3->setFrameShadow(QFrame::Raised);
    _events_views.insert(std::pair<unsigned,EventFrame*>(_generator,TestFrame3));
}

CentralFrame::~CentralFrame()
{
    delete ui;
}

void CentralFrame::AddEvent(Event *event)
{
    auto newFrame = new EventFrame(_generator++,event,this);
    newFrame->setFrameShape(QFrame::StyledPanel);
    newFrame->setFrameShadow(QFrame::Raised);
    _events_views.insert(std::pair<unsigned,EventFrame*>(_generator,newFrame));
}




