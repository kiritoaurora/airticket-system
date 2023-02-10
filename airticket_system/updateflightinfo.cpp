#include "updateflightinfo.h"
#include "ui_updateflightinfo.h"
#include <QDebug>

updateFlightInfo::updateFlightInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::updateFlightInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("修改航班信息");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
}

updateFlightInfo::~updateFlightInfo()
{
    delete ui;
}

void updateFlightInfo::setInfo()
{
    ui->lineEdit->setText(info[0]);
    ui->lineEdit_2->setText(info[1]);
    ui->lineEdit_3->setText(info[2]);
    ui->lineEdit_4->setText(info[3]);
    ui->lineEdit_5->setText(info[4]);
    ui->lineEdit_6->setText(info[5]);
    ui->lineEdit_7->setText(info[6]);
    ui->lineEdit_8->setText(info[7]);
    ui->lineEdit_9->setText(info[8]);
}

void updateFlightInfo::on_pushButton_clicked()  //更新按钮
{
    info[5] = ui->lineEdit_6->text();
    info[6] = ui->lineEdit_7->text();
    info[8] = ui->lineEdit_9->text();
    accept();
}

void updateFlightInfo::on_pushButton_2_clicked() //取消按钮
{
    this->close();
}
