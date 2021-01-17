#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<timemaster.h>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <textformatcontrol.h>
#include "newevent_View.h"
#include "newtimeline_View.h"
#include "eventframe_View.h"
#include "centralframe_View.h"
#include <QStringListModel>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QString GetActiveRow();
    QString SetFile();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TimeMaster *_timeEngine;
    CentralFrame *_centralFrame;
    QStringListModel *tagList;

private slots:


    void on_actionEmpty_project_triggered();

    void on_AddTagButt_clicked();

    void on_actionAdd_timeline_triggered();

    void on_actionAdd_Event_triggered();

    void on_actionAdd_Tag_triggered();

    void on_TagList_doubleClicked(const QModelIndex &index);

    void on_EraseTagButt_2_clicked();

    void on_actionErase_selected_triggered();



    void on_SelectButton_clicked();

    void on_SelectOnlyButton_clicked();

    void on_UnselectButton_clicked();

    void on_HideButton_clicked();

    void on_ShowButton_clicked();

    void on_ShowAllButton_clicked();

    void on_actionChange_resolution_triggered();

    void on_actionChange_color_triggered();

    void on_actionReset_selection_triggered();

    void on_actionChange_selected_color_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

signals:

};
#endif // MAINWINDOW_H
