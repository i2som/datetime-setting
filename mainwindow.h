#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>

#include <QDebug>
#include <QScreen>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *timer;
    QDateTime rtc_time;
    bool change_time_flag;

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void time_update();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();


private:
    Ui::MainWindow *ui;
    void rtctime_update();

};

#endif // MAINWINDOW_H
