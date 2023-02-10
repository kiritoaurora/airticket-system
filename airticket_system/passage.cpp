#include "passage.h"
#include "ui_passage.h"

Passage::Passage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Passage)
{
    ui->setupUi(this);
    this->setWindowTitle("添加信息界面");
    ui->lineEdit->setStyleSheet("border-radius:15px");
    ui->lineEdit_2->setStyleSheet("border-radius:15px");
    ui->lineEdit_3->setStyleSheet("border-radius:15px");
}

Passage::~Passage()
{
    delete ui;
}

void Passage::on_pushButton_clicked()
{

}

void Passage::on_pushButton_2_clicked()
{
    this->close();
}
