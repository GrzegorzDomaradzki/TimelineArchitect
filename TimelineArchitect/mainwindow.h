#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<timemaster.h>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <textformatcontrol.h>
#include "newevent.h"
#include "newtimeline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TimeMaster *timeEngine;

private slots:



    void on_actionEmpty_project_triggered();

    void on_AddTagButt_clicked();

    void on_actionAdd_timeline_triggered();

    void on_actionAdd_Event_triggered();

signals:

};
#endif // MAINWINDOW_H
