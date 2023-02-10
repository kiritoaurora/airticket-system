#include "adminsearch.h"
#include "ui_adminsearch.h"
#include <QDateTime>

adminSearch::adminSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminSearch)
{
    ui->setupUi(this);
    this->setWindowTitle("航班信息查询");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    QDateTime currentDate = QDateTime::currentDateTime();
    QString year = currentDate.toString("yyyy");
    QString month = currentDate.toString("MM");
    QString day = currentDate.toString("dd");
    setCombox(year,month,day);
}

adminSearch::~adminSearch()
{
    delete ui;
}

void adminSearch::setCombox(QString year,QString month,QString day) //设置combox日期为系统日期
{
    ui->comboBox->setCurrentText(year);
    for(int i = 0 ; i < 12 ; i++)
    {
        if(month == comboxMonth[i])
            ui->comboBox_2->setCurrentIndex(i);
    }
    for(int j = 0;j < 31; j++)
    {
        if(day == comboxDay[j])
            ui->comboBox_3->setCurrentIndex(j);
    }
}

void adminSearch::on_pushButton_clicked()  //查询按钮
{
    QString year,month,day;
    oStation = ui->lineEdit->text();
    eStation = ui->lineEdit_2->text();
    year = ui->comboBox->currentText();
    month = ui->comboBox_2->currentText();
    day = ui->comboBox_3->currentText();
    date = year + "-" + month + "-" + day;
    airnum = ui->lineEdit_3->text();
    accept();
}

void adminSearch::on_pushButton_2_clicked()
{
    this->close();
}
