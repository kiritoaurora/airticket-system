#include "search.h"
#include "ui_search.h"
#include "widget.h"
#include "ui_widget.h"
#include "book_ticket.h"
#include "ui_book_ticket.h"
#include "changepassword.h"
#include "ui_changepassword.h"
#include "myorder.h"
#include "ui_myorder.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
search::search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::search)
{
    ui->setupUi(this);
    this->setWindowTitle("查询界面");
    this->setMouseTracking(true);
    ui->pushButton_2->setMouseTracking(true);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    //设置tableWidget列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->lineEdit->setFocus();
    ui->widget->hide();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString year = currentDate.toString("yyyy");    
    QString month = currentDate.toString("MM");
    QString day = currentDate.toString("dd");
    setCombox(year,month,day);
}

search::~search()
{
    delete ui;
}
/******************************************************************/
//鼠标移动到用户按钮上显示widget
/******************************************************************/
void search::mouseMoveEvent(QMouseEvent * e)
{
    if(uid != ""&&e->x()>=1432&&e->x()<=1513&&e->y()>=10&&e->y()<=91)
        ui->widget->show();
    if(!(e->x()>=1050&&e->x()<=1513&&e->y()>=10&&e->y()<=91))
        ui->widget->hide();
}
/******************************************************************/
//设置combox日期为系统日期
/******************************************************************/
void search::setCombox(QString year,QString month,QString day)
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
/******************************************************************/
//判断是否处于登陆状态，根据登陆与否决定用户按钮的输出文字
/******************************************************************/
void search::isLogin(bool flag)
{
    if(flag == false)
        ui->pushButton_2->setText("登录");
    else if(flag == true)
        ui->pushButton_2->setText("用户");
}
/******************************************************************/
//根据决定用户按钮的文字，判断是否处于登陆状态
/******************************************************************/
void search::on_pushButton_2_clicked() //用户按钮
{
    QString str;
    str = ui->pushButton_2->text();
    if(str == "登录")
    {
        this->close();
        Widget *login = new Widget;
        login->show();
    }
    else
    {
        ui->widget->show();
    }
}
/******************************************************************/
//根据输入的起点终点和日期，对数据库进行遍历输出相应的航班信息
/******************************************************************/
void search::on_pushButton_clicked()   //查询按钮
{
    ui->tableWidget->setRowCount(0);  //初始化tableWidget行数为0
    QString oStation,eStation,year,month,day; //获取输入的信息
    oStation = ui->lineEdit->text();
    eStation = ui->lineEdit_2->text();
    year = ui->comboBox->currentText();
    month = ui->comboBox_2->currentText();
    day = ui->comboBox_3->currentText();
    QString sDate;
    sDate = year + "-" + month + "-" + day;  //拼接日期
    QSqlQuery query;
    //调用存储过程，判断航班日期与当前日期相差天数是否小于4,若小于则使价格增长25%
    QString sql1 = QString("exec priceFluctuation");
    query.exec(sql1);
    //调用存储过程，根据起点终点和日期在视图flightInformation中查询信息
    QString sql = QString("exec searchFlight '%1','%2','%3'")
            .arg(oStation).arg(eStation).arg(sDate);    
    query.exec(sql);
    //将查询到的结果输出到tableWidget中，并设置居中
    for(int i = 0;query.next();i++)
    {
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        for(int j = 0;j < 12;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    QString sql2 = QString("exec rePriceFluctuation");
    query.exec(sql2);
}

void search::on_pushButton_3_clicked()  //转换起点终点的输入值
{
    QString ost,est;
    ost = ui->lineEdit->text();
    est = ui->lineEdit_2->text();
    ui->lineEdit->setText(est);
    ui->lineEdit_2->setText(ost);
}
/******************************************************************/
//选中航班信息并双击，将对应航班信息输入到购票界面的tableWidget中，显示购票界面
//若未登录则发出提示
/******************************************************************/
void search::on_tableWidget_cellDoubleClicked(int row, int column) //双击航班进入订票界面
{
    column = 0;
    QDateTime date = QDateTime::currentDateTime();
    QString currentTime = date.currentDateTime().toString("hh:mm:ss");
    QString currentDate = date.currentDateTime().toString("yyyy-MM-dd");
    if(uid=="")
    {
       QMessageBox::information(this,"提示信息","您尚未登陆！");
    }
    else if(currentDate>ui->tableWidget->item(row,3)->text()||(currentDate==ui->tableWidget->item(row,3)->text()&&currentTime>ui->tableWidget->item(row,4)->text()))
    {
        QMessageBox::information(this,"提示信息","航班已过期！");
        qDebug()<<currentDate<<ui->tableWidget->item(row,4)->text();
    }
    else
    {
        book_ticket *book_widget = new book_ticket;
        book_widget->airnum = ui->tableWidget->item(row,0)->text();
        book_widget->oStation = ui->tableWidget->item(row,1)->text();
        book_widget->eStation = ui->tableWidget->item(row,2)->text();
        book_widget->date = ui->tableWidget->item(row,3)->text();
        book_widget->sTime = ui->tableWidget->item(row,4)->text();
        book_widget->eTime = ui->tableWidget->item(row,5)->text();
        book_widget->fprice = ui->tableWidget->item(row,9)->text();
        book_widget->sprice = ui->tableWidget->item(row,10)->text();
        book_widget->cName = ui->tableWidget->item(row,11)->text();
        book_widget->uid = uid;  //传递用户名
        book_widget->setInfo();  //将选中航班信息输出到book_widget中的tableWidget中
        book_widget->show();
    }

}

void search::on_pushButton_6_clicked()    //退出登录
{
    uid = "";
    ui->pushButton_2->setText("登录");
    ui->widget->hide();
}

void search::on_pushButton_5_clicked()   //修改密码按钮
{
    ui->widget->hide();
    changePassword *changePwd_widget = new changePassword;
    changePwd_widget->uid = uid;
    changePwd_widget->show();
}

void search::on_pushButton_4_clicked()   //我的订单
{
    ui->widget->hide();
    myOrder *order_widget = new myOrder;
    order_widget->uid = uid;   //传递用户名给order_widget
    order_widget->setInfo();  //将用户名下所有订单显示在tablewidget中
    order_widget->show();
}
