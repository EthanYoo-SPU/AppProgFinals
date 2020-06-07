#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTimeZone>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setCurrentTime();
    void setWorldTime();

    void on_timeZoneBox_activated(int index);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void changeGreetingLabel();
    int timeZoneDifference = 0;
    QTimeZone pacificTime = QTimeZone(0);
    QTimeZone easternTime = QTimeZone(10800);
    QTimeZone koreaTime = QTimeZone(-28800);


};
#endif // MAINWINDOW_H
