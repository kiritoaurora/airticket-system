#include "passages.h"
#include "ui_passages.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>

Passages::Passages(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Passages)
{
    ui->setupUi(this);
    this->setWindowTitle("添加信息界面");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
}

Passages::~Passages()
{
    delete ui;
}

void Passages::on_pushButton_clicked()   //确认按钮
{
    id = ui->lineEdit->text();
    name = ui->lineEdit_2->text();
    pnum = ui->lineEdit_3->text();

    QString year,month,day,days;
    year = id.mid(6,4);
    month = id.mid(10,2);
    day = id.mid(12,2);

    QDate currentDate = QDate::currentDate();
    QString nowYear = currentDate.currentDate().toString("yyyy");//获取当前年份

    int year2 = year.toInt();
    bool isLeapYear = QDate::isLeapYear(year2); //判断是不是闰年

    if(month == "01"||month == "03"||month == "05"||month == "07"||month == "08"||month == "10"||month == "12")
        days = "31";   //一、三、五、七、八、十、十二有31天
    else if(month == "04"||month == "06"||month == "09"||month == "11")
            days = "30";  //四、六、九、十一有30天
    else if(month == "02"&&isLeapYear == true)
        days = "29";   //闰年二月有29天
    else
        days = "28";   //平年二月有28天
    if(id == ""||name == ""||pnum == "")
        QMessageBox::information(this,"乘机人信息","信息填写不完整！");
    else if(id.length() != 18||year>nowYear||month>"13"||month<"01"||day>days)
        QMessageBox::information(this,"乘机人信息","ID填写不正确！");
    else
        accept();
}

void Passages::on_pushButton_2_clicked()  //取消按钮
{
    this->close();
}
