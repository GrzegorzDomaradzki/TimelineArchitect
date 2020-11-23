#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<timemaster.h>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <textformatcontrol.h>

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

signals:

};
#endif // MAINWINDOW_H
