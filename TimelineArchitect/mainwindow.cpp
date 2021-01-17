#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QStringList>
#include <QColorDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _timeEngine = new TimeMaster(this);
    _centralFrame = new CentralFrame(centralWidget());
    _centralFrame->setObjectName(QString::fromUtf8("frame"));
    _centralFrame->setFrameShape(QFrame::StyledPanel);
    _centralFrame->setFrameShadow(QFrame::Raised);
    _centralFrame->timeEngine = _timeEngine;
    ui->verticalLayout_4->addWidget(_centralFrame);
    tagList = new QStringListModel(this);
    ui->TagList->setModel(tagList);
    ui->TagList->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

QString MainWindow::GetActiveRow()
{
    auto row = ui->TagList->currentIndex().row();
    if (row == -1) return "";
    return tagList->data(tagList->index(row)).toString();
}

QString MainWindow::SetFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::currentPath(),
                               tr("save files (*.time)"));
    return fileName;

}



MainWindow::~MainWindow()
{
    delete  _centralFrame;
    delete  _timeEngine;
    delete ui;

}




void MainWindow::on_actionEmpty_project_triggered()
{
    auto reply = QMessageBox::question(this, "New Project", "Are you sure? All unsaved data will be lost",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply!=QMessageBox::Yes) return;
    if (_timeEngine!=nullptr)
    {
        delete _timeEngine;
        _timeEngine = nullptr;
    }
    _timeEngine = new TimeMaster(this);
    _centralFrame = new CentralFrame(centralWidget());
    _centralFrame->timeEngine = _timeEngine;
    _centralFrame->Purge();
    _centralFrame->UpdateTimelineData();
}

void MainWindow::on_AddTagButt_clicked()
{
    QString text = QInputDialog::getText(this, tr("Add tag"),
                                             tr("Tag name:"), QLineEdit::Normal);
    if (text.isEmpty()) return;
    QString info;
    if (TextFormatControl::TagName(text,info))
    {
        if (_timeEngine->tags->RegisterTag(text)!=-1)
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
    dialog.SetMaster(_timeEngine);
    dialog.exec();

    _centralFrame->UpdateTimelineData();
}

void MainWindow::on_actionAdd_Event_triggered()
{
    if (_timeEngine->TimelineCount()==0)
    {
        QString info = "First create timeline";
        QMessageBox msgBox;
        msgBox.setText(info);
        msgBox.exec();
        return;
    }
    NewEvent dialog(true);
    dialog.SetMaster(_timeEngine);
    dialog.SetDateStart(_timeEngine->GetTimeline(0)->GetStart().addDays(1));
    dialog.SetDateEnd(_timeEngine->GetTimeline(_timeEngine->TimelineCount()-1)->GetEnd().addDays(-1));
    dialog.exec();
    Event* event = dialog.GetEvent();
    if (event!=nullptr) _centralFrame->AddEvent(event);

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
        if (_timeEngine->tags->RenameTag(old,text)!=-1)
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
    if (-1==_timeEngine->tags->DeleteTag(tagText))
    {
        QMessageBox msgBox;
        msgBox.setText("Error: erased tag never existed - consider reporting issue");
        msgBox.exec();
    }
    tagList->removeRow(row);
}

void MainWindow::on_actionErase_selected_triggered()
{
    _centralFrame->EraseSelected();
}



void MainWindow::on_SelectButton_clicked()
{
    auto tagText = GetActiveRow();
    if (tagText=="") return;
    auto IDs = _timeEngine->getTagOwners(tagText);
    foreach (auto id , IDs) _centralFrame->AddHim(id,1);
}

void MainWindow::on_SelectOnlyButton_clicked()
{
    auto tagText = GetActiveRow();
    if (tagText=="") return;
    auto IDs = _timeEngine->getTagOwners(tagText);
    _centralFrame->UnselectAll();
    foreach (auto id , IDs) _centralFrame->AddHim(id,1);
}

void MainWindow::on_UnselectButton_clicked()
{
    auto tagText = GetActiveRow();
    if (tagText=="") return;
    auto IDs = _timeEngine->getTagOwners(tagText);
    foreach (auto id , IDs) _centralFrame->AddHim(id,0);
}

void MainWindow::on_HideButton_clicked()
{
    auto tagText = GetActiveRow();
    if (tagText=="") return;
    _centralFrame->HideGiven(_timeEngine->getTagOwners(tagText));
}

void MainWindow::on_ShowButton_clicked()
{
    auto tagText = GetActiveRow();
    if (tagText=="") return;
    _centralFrame->ShowGiven(_timeEngine->getTagOwners(tagText));
}

void MainWindow::on_ShowAllButton_clicked()
{
    _centralFrame->ShowAll();
}

void MainWindow::on_actionChange_resolution_triggered()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Set resolution (10 is default) "),
                                    tr("Resolution"), 10, 5, 100, 1, &ok);
    if (ok && i!=_centralFrame->GetResolution()) _centralFrame->SetResolution(i);
}

void MainWindow::on_actionChange_color_triggered()
{
    QColor color = QColorDialog::getColor(_centralFrame->color,this);
    if (color.isValid())
    {
    _centralFrame->color = color;
    _centralFrame->Redraw();
    }
}

void MainWindow::on_actionReset_selection_triggered()
{
    _centralFrame->UnselectAll();
}

void MainWindow::on_actionChange_selected_color_triggered()
{
    QColor color = QColorDialog::getColor(_centralFrame->GetSelectedColor(),this);
    if (color.isValid() && color!=_centralFrame->GetSelectedColor())
    {
    _centralFrame->SetSelectedColor(color);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (_timeEngine->Filename.isEmpty())
    {
        auto fileName = SetFile();
        if(fileName.isEmpty()) return;
        _timeEngine->Filename = fileName;
    }
    if (-1==_timeEngine->SaveProject())
    {
            QMessageBox msgBox;
            msgBox.setText("file can't be opened");
            msgBox.exec();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    auto newName = SetFile();
    if (newName.isEmpty()) return;
    _timeEngine->Filename = newName;
    on_actionSave_triggered();
}
