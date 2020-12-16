#include "centralframe_View.h"
#include "ui_centralframe.h"



CentralFrame::CentralFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CentralFrame)
{
    ui->setupUi(this);
    generator = 0;

    auto TestFrame1 =new EventFrame(generator++, this);
    TestFrame1->setFrameShape(QFrame::StyledPanel);
    TestFrame1->setFrameShadow(QFrame::Raised);
    events_views.insert(std::pair<unsigned,EventFrame*>(generator,TestFrame1));
    connect(TestFrame1,SIGNAL(GiveMeStage(unsigned)),this,SLOT(OnStageRequest(unsigned)));

    auto TestFrame2 = new EventFrame(generator++,this);
    TestFrame2->setFrameShape(QFrame::StyledPanel);
    TestFrame2->setFrameShadow(QFrame::Raised);
    events_views.insert(std::pair<unsigned,EventFrame*>(generator,TestFrame2));
    connect(TestFrame2,SIGNAL(GiveMeStage(unsigned)),this,SLOT(OnStageRequest(unsigned)));

    auto TestFrame3 =new EventFrame(generator++,this);
    TestFrame3->setFrameShape(QFrame::StyledPanel);
    TestFrame3->setFrameShadow(QFrame::Raised);
    events_views.insert(std::pair<unsigned,EventFrame*>(generator,TestFrame3));
    connect(TestFrame3,SIGNAL(GiveMeStage(unsigned)),this,SLOT(OnStageRequest(unsigned)));
}

CentralFrame::~CentralFrame()
{
    delete ui;
}

void CentralFrame::OnStageRequest(unsigned id)
{
    auto child = events_views[id];
}



