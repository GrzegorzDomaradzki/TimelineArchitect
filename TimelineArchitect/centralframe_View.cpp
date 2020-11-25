#include "centralframe_View.h"
#include "ui_centralframe.h"

static inline QString fridgetMagnetsMimeType() { return QStringLiteral("application/x-eventframe_View"); }

CentralFrame::CentralFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CentralFrame)
{
    ui->setupUi(this);

    auto TestFrame1 =new EventFrame(this);
    TestFrame1->setFrameShape(QFrame::StyledPanel);
    TestFrame1->setFrameShadow(QFrame::Raised);
    events_views.push_back(TestFrame1);

    auto TestFrame2 = new EventFrame(this);
    TestFrame2->setFrameShape(QFrame::StyledPanel);
    TestFrame2->setFrameShadow(QFrame::Raised);
    events_views.push_back(TestFrame2);

    auto TestFrame3 =new EventFrame(this);
    TestFrame3->setFrameShape(QFrame::StyledPanel);
    TestFrame3->setFrameShadow(QFrame::Raised);
    events_views.push_back(TestFrame3);
}

CentralFrame::~CentralFrame()
{
    delete ui;
}



