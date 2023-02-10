#include "checkorder.h"
#include "ui_checkorder.h"

checkOrder::checkOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkOrder)
{
    ui->setupUi(this);
    this->setWindowTitle("确认界面");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
}

checkOrder::~checkOrder()
{
    delete ui;
}

void checkOrder::setInfo()
{
    ui->lineEdit->setText(airnum);
    ui->lineEdit_2->setText(oStation);
    ui->lineEdit_3->setText(eStation);
    ui->lineEdit_4->setText(date);
    ui->lineEdit_5->setText(sTime);
    ui->lineEdit_6->setText(eTime);
    ui->lineEdit_7->setText(cName);
    ui->lineEdit_8->setText(total_passages);
    ui->lineEdit_9->setText(total_money);
    ui->lineEdit_10->setText(ordernum);
}

void checkOrder::on_pushButton_clicked()  //确认按钮
{
    accept();
}

void checkOrder::on_pushButton_2_clicked()  //取消按钮
{
    close();
}
