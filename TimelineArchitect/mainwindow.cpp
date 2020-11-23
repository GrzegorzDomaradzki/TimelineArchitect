#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeEngine = new TimeMaster(this);



}



MainWindow::~MainWindow()
{
    delete  timeEngine;
    delete ui;
}




void MainWindow::on_actionEmpty_project_triggered()
{
    if (timeEngine!=nullptr)
    {
        delete timeEngine;
        timeEngine = nullptr;
    }
    timeEngine = new TimeMaster(this);
    qDebug() << "New Timeline";
}

void MainWindow::on_AddTagButt_clicked()
{
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("User name:"), QLineEdit::Normal);
    if (text.isEmpty()) return;
    QString info;
    if (TextFormatControl::TagName(text,info))
    {
        if (timeEngine->tags->RegisterTag(text)!=-1) return;
        info = "This tagName alredy exist";
    }
    QMessageBox msgBox;
    msgBox.setText(info);
    msgBox.exec();
}

void MainWindow::on_actionAdd_timeline_triggered()
{
    NewTimeline dialog;
    dialog.setModal(true);
    dialog.SetMaster(timeEngine);
    dialog.exec();
}

void MainWindow::on_actionAdd_Event_triggered()
{
    NewEvent dialog;
    dialog.setModal(true);
    dialog.SetMaster(timeEngine);
    dialog.exec();
}
