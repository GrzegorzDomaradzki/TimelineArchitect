#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QStringList>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeEngine = new TimeMaster(this);
    centralFrame = new CentralFrame(centralWidget());
    centralFrame->setObjectName(QString::fromUtf8("frame"));
    centralFrame->setFrameShape(QFrame::StyledPanel);
    centralFrame->setFrameShadow(QFrame::Raised);
    ui->verticalLayout_4->addWidget(centralFrame);
    tagList = new QStringListModel(this);
    ui->TagList->setModel(tagList);
    ui->TagList->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    QString text = QInputDialog::getText(this, tr("Add tag"),
                                             tr("Tag name:"), QLineEdit::Normal);
    if (text.isEmpty()) return;
    QString info;
    if (TextFormatControl::TagName(text,info))
    {
        if (timeEngine->tags->RegisterTag(text)!=-1)
        {
            tagList->insertRow(0);
            auto index = tagList->index(0);
            tagList->setData(index,text);
            tagList->sort(0);
            return;
        }
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
    if (timeEngine->EventCount()==0)
    {
        QString info = "First create timeline";
        QMessageBox msgBox;
        msgBox.setText(info);
        msgBox.exec();
        return;
    }
    NewEvent dialog;
    dialog.setModal(true);
    dialog.SetMaster(timeEngine);
    dialog.exec();
}

void MainWindow::on_actionAdd_Tag_triggered()
{
    on_AddTagButt_clicked();
}

void MainWindow::on_TagList_doubleClicked(const QModelIndex &index)
{
    QString old = tagList->data(index).toString();

    QString text = QInputDialog::getText(this, "Rename tag " + old,
                                             tr("Tag name:"), QLineEdit::Normal);
    if (text.isEmpty()) return;
    QString info;
    if (TextFormatControl::TagName(text,info))
    {
        if (timeEngine->tags->RenameTag(old,text)!=-1)
        {
            tagList->setData(index,text);
            tagList->sort(0);
            return;
        }
        info = "This tagName alredy exist";
    }
    QMessageBox msgBox;
    msgBox.setText(info);
    msgBox.exec();
}

void MainWindow::on_EraseTagButt_2_clicked()
{
    auto row = ui->TagList->currentIndex().row();
    if (row == -1) return;
    auto tagText = tagList->data(tagList->index(row)).toString();
    QString question = "Delete ";
    question+=tagText + " tag?";
    auto reply = QMessageBox::question(this, "Erase Tag", question,
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply!=QMessageBox::Yes) return;
    if (-1==timeEngine->tags->DeleteTag(tagText))
    {
        QMessageBox msgBox;
        msgBox.setText("Error: erased tag never existed - consider reporting issue");
        msgBox.exec();
    }
    tagList->removeRow(row);
}
