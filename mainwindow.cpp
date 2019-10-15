#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QProcess>
#include <QStyle>
#include <QFile>
#include <QTextStream>

// #define debug_open
#ifdef debug_open
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    change_time_flag(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer;
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lcdNumber->adjustSize();

    QSize iconSize(36, 36);
    ui->toolButton->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    ui->toolButton->setIconSize(iconSize);
    ui->toolButton_2->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->toolButton_2->setIconSize(iconSize);
    ui->toolButton_3->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    ui->toolButton_3->setIconSize(iconSize);
    ui->toolButton_4->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    ui->toolButton_4->setIconSize(iconSize);

    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
//    ui->dateTimeEdit->setFocus();
//    ui->dateTimeEdit->setSelectedSection(ui->dateTimeEdit->sectionAt(0));

    rtctime_update();
    ui->lcdNumber_2->setDigitCount(19);
    ui->lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_2->display(rtc_time.toString("yyyy-MM-dd hh:mm:ss"));
    ui->lcdNumber_2->adjustSize();

    connect(timer,SIGNAL(timeout()),this,SLOT(time_update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::time_update()
{
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if (!change_time_flag)
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    rtctime_update();
    ui->lcdNumber_2->display(rtc_time.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::rtctime_update()
{
    QFile file("/proc/driver/rtc");
    // QFile file("F:/study/my/xy_pro/20191011_embedded_demos/build-clock_set-Desktop_Qt_5_6_2_MinGW_32bit-Debug/debug/rtc");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("file open failed\r\n");
        return;
    }
    QString tmp = file.readAll();
    int flag = 0;
    QStringList list = tmp.split("\n");

    foreach(QString str, list) {
        QString line = str.trimmed();
        if (line.startsWith("rtc_time")) {
#ifdef debug_open
            qDebug() << line.section(':', 1).trimmed();
#endif
            QTime tmp;
            tmp = tmp.fromString(line.section(':', 1).trimmed());
            rtc_time.setTime(tmp);
            flag++;
        }
        if (line.startsWith("rtc_date")) {
#ifdef debug_open
            qDebug() << line.section(':', 1).trimmed();
#endif
            QDate tmp;
            tmp = tmp.fromString(line.section(':', 1).trimmed(), "yyyy-MM-dd");
            rtc_time.setDate(tmp);
            flag++;
        }
        if (flag >= 2)
            break;
    }
    file.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    close();
}

void MainWindow::on_pushButton_clicked()
{
#if 0
    QProcess process;
    process.start("hwclock -s");
    process.waitForFinished();

#ifdef debug_open
    QByteArray output = process.readAllStandardOutput();
    QString str_output = output;
    qDebug() << output;
    qDebug() << process.exitStatus();
#endif
#endif


    if (change_time_flag) {
        QString new_time(ui->dateTimeEdit->dateTime().toString("yyyyMMdd HH:mm:ss"));
        QString cmd = "date -s \"" + new_time + "\"";
    #ifdef debug_open
        qDebug() << "sys time set: " << cmd;
    #endif

        QProcess process;
        process.start(cmd);
        process.waitForFinished();

    #ifdef debug_open
        QByteArray output = process.readAllStandardOutput();
        QString str_output = output;
        qDebug() << output;
        qDebug() << process.exitStatus();
    #endif
        change_time_flag = false;
    }

}

void MainWindow::on_pushButton_2_clicked()
{

    QProcess process;
    process.start("hwclock -w");
    process.waitForFinished();

#ifdef debug_open
    QByteArray output = process.readAllStandardOutput();
    QString str_output = output;
    qDebug() << output;
    qDebug() << process.exitStatus();
#endif

    rtctime_update();
    ui->lcdNumber_2->display(rtc_time.toString("yyyy-MM-dd hh:mm:ss"));

}

void MainWindow::on_toolButton_clicked()
{
    if (change_time_flag) {
        int index = ui->dateTimeEdit->currentSectionIndex();
        ui->dateTimeEdit->setCurrentSectionIndex(index - 1);
        ui->dateTimeEdit->setSelectedSection(ui->dateTimeEdit->currentSection());
    } else {
        ui->dateTimeEdit->setFocus();
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::SecondSection);
        change_time_flag = true;
    }

}

void MainWindow::on_toolButton_2_clicked()
{
    if (change_time_flag) {
        switch (ui->dateTimeEdit->currentSection()) {
        case QDateTimeEdit::YearSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addYears(1));
            break;
        case QDateTimeEdit::MonthSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addMonths(1));
            break;
        case QDateTimeEdit::DaySection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addDays(1));
            break;
        case QDateTimeEdit::HourSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(3600));
            break;
        case QDateTimeEdit::MinuteSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(60));
            break;
        case QDateTimeEdit::SecondSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(1));
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_toolButton_3_clicked()
{
    if (change_time_flag) {
        switch (ui->dateTimeEdit->currentSection()) {
        case QDateTimeEdit::YearSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addYears(-1));
            break;
        case QDateTimeEdit::MonthSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addMonths(-1));
            break;
        case QDateTimeEdit::DaySection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addDays(-1));
            break;
        case QDateTimeEdit::HourSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(-3600));
            break;
        case QDateTimeEdit::MinuteSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(-60));
            break;
        case QDateTimeEdit::SecondSection:
            ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(-1));
            break;
        default:
            break;
        }
    }
}
void MainWindow::on_toolButton_4_clicked()
{
    if (change_time_flag) {
        int index = ui->dateTimeEdit->currentSectionIndex();
        ui->dateTimeEdit->setCurrentSectionIndex(index + 1);
        ui->dateTimeEdit->setSelectedSection(ui->dateTimeEdit->currentSection());
    } else {
        ui->dateTimeEdit->setFocus();
        ui->dateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection);
        change_time_flag = true;
    }
}