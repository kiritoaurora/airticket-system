#include "orderinfo.h"
#include "ui_orderinfo.h"
#include "myorder.h"
#include "ui_myorder.h"
#include "a320_seattable.h"
#include "ui_a320_seattable.h"
#include "b737_seattable.h"
#include "ui_b737_seattable.h"
#include "a330_seattable.h"
#include "ui_a330_seattable.h"
#include "a320_seattable_2.h"
#include "ui_a320_seattable_2.h"
#include "b737_seattable_2.h"
#include "ui_b737_seattable_2.h"
#include "a330_seattable_2.h"
#include "ui_a330_seattable_2.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

orderInfo::orderInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::orderInfo)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    this->setWindowTitle("订单信息界面");
    //设置tableWidget列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget->verticalHeader()->hide();
}

orderInfo::~orderInfo()
{
    delete ui;
}

void orderInfo::setInfo()  //将订单中的机票信息显示在tablewidget中
{
    QSqlQuery query;
    QString sql = QString("select * from ticketInformation where ordernum = '%1'")
            .arg(ordernum);
    query.exec(sql);
    for(int i = 0;query.next();i++)
    {
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        for(int j = 0;j < 10;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

void orderInfo::saveSeatNum(QString ticketNum,QString seatNum) //保存座位号到数据库中
{
    QString sql = QString("update ticket set seatnum = '%1' where ticketnum = '%2'")
            .arg(seatNum).arg(ticketNum);
    QSqlQuery query;
    query.exec(sql);
}

void orderInfo::A320_firstLevel()   //机型为空客320的商务舱选座函数
{
    A320_seatTable *A320_firstClass = new A320_seatTable;
    A320_firstClass->amount = ui->tableWidget->rowCount();  //机票数
    A320_firstClass->airnum = ui->tableWidget->item(0,1)->text(); //航班号
    A320_firstClass->date = ui->tableWidget->item(0,4)->text();  //日期
    A320_firstClass->class_level = ui->tableWidget->item(0,8)->text();  //舱位等级
    A320_firstClass->createArray();  //根据机票数创建对应长度的数组用于存放下标
    A320_firstClass->fillTable(); //扫描对应航班和日期的机票信息，填充二维数组seat_table
    A320_firstClass->update();  //重绘界面
    A320_firstClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(A320_firstClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = A320_firstClass->seatnum[i]; //选中的座位号
            QString tNum = ui->tableWidget->item(i,0)->text();  //机票号
            saveSeatNum(tNum,seat);  //将座位号存入数据库
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        A320_firstClass->close();
    }
}

void orderInfo::A320_secondLevel()    //机型为空客320的经济舱选座函数
{
    A320_seatTable_2 *A320_secondClass = new A320_seatTable_2;
    A320_secondClass->amount = ui->tableWidget->rowCount();
    A320_secondClass->airnum = ui->tableWidget->item(0,1)->text();
    A320_secondClass->date = ui->tableWidget->item(0,4)->text();
    A320_secondClass->class_level = ui->tableWidget->item(0,8)->text();
    A320_secondClass->createArray(); //根据机票数创建对应长度的数组用于存放下标
    A320_secondClass->fillTable(); //扫描对应航班和日期的机票信息，填充二维数组seat_table
    A320_secondClass->update();
    A320_secondClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(A320_secondClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = A320_secondClass->seatnum[i];
            QString tNum = ui->tableWidget->item(i,0)->text();
            saveSeatNum(tNum,seat);
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        A320_secondClass->close();
    }
}

void orderInfo::A330_firstLevel()   //机型为空客330的商务舱选座函数
{
    A330_seatTable *A330_firstClass = new A330_seatTable;
    A330_firstClass->amount = ui->tableWidget->rowCount();
    A330_firstClass->airnum = ui->tableWidget->item(0,1)->text();
    A330_firstClass->date = ui->tableWidget->item(0,4)->text();
    A330_firstClass->class_level = ui->tableWidget->item(0,8)->text();
    A330_firstClass->createArray();  //根据机票数创建对应长度的数组用于存放下标
    A330_firstClass->fillTable();  //扫描对应航班和日期的机票信息，填充二维数组seat_table
    A330_firstClass->update();
    A330_firstClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(A330_firstClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = A330_firstClass->seatnum[i];
            QString tNum = ui->tableWidget->item(i,0)->text();
            saveSeatNum(tNum,seat);
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        A330_firstClass->close();
    }
}

void orderInfo::A330_secondLevel()    //机型为空客330的经济舱选座函数
{
    A330_seatTable_2 *A330_secondClass = new A330_seatTable_2;
    A330_secondClass->amount = ui->tableWidget->rowCount();
    A330_secondClass->airnum = ui->tableWidget->item(0,1)->text();
    A330_secondClass->date = ui->tableWidget->item(0,4)->text();
    A330_secondClass->class_level = ui->tableWidget->item(0,8)->text();
    A330_secondClass->createArray(); //根据机票数创建对应长度的数组用于存放下标
    A330_secondClass->fillTable(); //扫描对应航班和日期的机票信息，填充二维数组seat_table
    A330_secondClass->update();
    A330_secondClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(A330_secondClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = A330_secondClass->seatnum[i];
            QString tNum = ui->tableWidget->item(i,0)->text();
            saveSeatNum(tNum,seat);
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        A330_secondClass->close();
    }
}

void orderInfo::B737_firstLevel()   //机型为波音737的商务舱选座函数
{
    B737_seatTable *B737_firstClass = new B737_seatTable;
    B737_firstClass->amount = ui->tableWidget->rowCount();
    B737_firstClass->airnum = ui->tableWidget->item(0,1)->text();
    B737_firstClass->date = ui->tableWidget->item(0,4)->text();
    B737_firstClass->class_level = ui->tableWidget->item(0,8)->text();
    B737_firstClass->createArray(); //根据机票数创建对应长度的数组用于存放下标
    B737_firstClass->fillTable(); //扫描对应航班和日期的机票信息，填充二维数组seat_table
    B737_firstClass->update();
    B737_firstClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(B737_firstClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = B737_firstClass->seatnum[i];
            QString tNum = ui->tableWidget->item(i,0)->text();
            saveSeatNum(tNum,seat);
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        B737_firstClass->close();
    }
}

void orderInfo::B737_secondLevel()   //机型为波音737的商务舱选座函数
{
    B737_seatTable_2 *B737_secondClass = new B737_seatTable_2;
    B737_secondClass->amount = ui->tableWidget->rowCount();
    B737_secondClass->airnum = ui->tableWidget->item(0,1)->text();
    B737_secondClass->date = ui->tableWidget->item(0,4)->text();
    B737_secondClass->class_level = ui->tableWidget->item(0,8)->text();
    B737_secondClass->createArray();  //根据机票数创建对应长度的数组用于存放下标
    B737_secondClass->fillTable();  //扫描对应航班和日期的机票信息，填充二维数组seat_table
    B737_secondClass->update();
    B737_secondClass->show();
    //选座成功，将座位号输出到tableWidget中，并将座位号输出到数据库中保存
    if(B737_secondClass->exec()==QDialog::Accepted)
    {
        for(int i =0;i<ui->tableWidget->rowCount();i++)
        {
            QString seat = B737_secondClass->seatnum[i];
            QString tNum = ui->tableWidget->item(i,0)->text();
            saveSeatNum(tNum,seat);
            ui->tableWidget->setItem(i,9,new QTableWidgetItem(seat));
            ui->tableWidget->item(i,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        B737_secondClass->close();
    }
}

void orderInfo::on_pushButton_clicked()  //值机按钮
{  
    QString date = ui->tableWidget->item(0,4)->text();
    QString sTime = ui->tableWidget->item(0,5)->text();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString nowDate = currentDate.toString("yyyy-MM-dd");
    QString nowTime = currentDate.toString("hh:mm:ss");
    //判定机票是否已过期
    if((nowDate > date) || ( nowDate == date && nowTime > sTime))
    {
        QMessageBox::information(this,"退票信息","机票已过期！值机无效！");
    }
    else if(ui->tableWidget->rowCount() == 0)  //无票
    {
        QMessageBox::information(this,"值机信息","值机操作无效！");
    }
    else if(ui->tableWidget->item(0,9)->text() != NULL)  //已选座
    {
        QMessageBox::information(this,"值机信息","您已完成值机！");
    }
    else   //满足选座条件
    {      
        QString aNum,date;
        aNum = ui->tableWidget->item(0,1)->text();
        date = ui->tableWidget->item(0,4)->text();
        //根据航班号和日期查询对应的飞机型号
        QString sql = QString("select plane_model from flight,airplane where "
                              "flight.planenum = airplane.planenum and "
                              "airnum = '%1' and date = '%2'").arg(aNum).arg(date);
        QSqlQuery query;
        query.exec(sql);
        query.next();
        QString plane_model = query.value(0).toString();
        QString class_level = ui->tableWidget->item(0,8)->text();
        //判定舱位等级，结合飞机型号调用相应的选座函数
        if(plane_model == "空客320" && class_level == "商务舱")
            A320_firstLevel();
        else if(plane_model == "空客320" && class_level == "经济舱")
            A320_secondLevel();
        else if(plane_model == "空客330" && class_level == "商务舱")
            A330_firstLevel();
        else if(plane_model == "空客330" && class_level == "经济舱")
            A330_secondLevel();
        else if(plane_model == "波音737" && class_level == "商务舱")
            B737_firstLevel();
        else if(plane_model == "波音737" && class_level == "经济舱")
            B737_secondLevel();
    }
}

void orderInfo::on_pushButton_2_clicked() //退票按钮
{
//    QString sTime = ui->tableWidget->item(0,5)->text();
//    QString nowTime = currentDate.toString("hh:mm:ss");
    //(nowDate > date) || ( nowDate == date && nowTime > sTime)
    QString date = ui->tableWidget->item(0,4)->text();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString nowDate = currentDate.toString("yyyy-MM-dd");
    QDate date1 = QDate().fromString(date,"yyyy-MM-dd");
    QDate date2 = QDate().fromString(nowDate,"yyyy-MM-dd");
    int days = date2.daysTo(date1);

    //判定机票是否已过期   "已失败"   "已出行"  "未出行"
    if(orderStatus == "已失败")
    {
        QMessageBox::information(this,"退票信息","订单已失败，操作无效！");
    }
    else if(orderStatus == "已出行"&&ui->tableWidget->item(0,9)->text() != NULL)
    {
        QMessageBox::information(this,"退票信息","机票已使用，操作无效！");
    }
    else if(orderStatus == "已出行"&&ui->tableWidget->item(0,9)->text() == NULL&&days > 3)
    {
        QMessageBox::information(this,"退票信息","机票过期，操作无效！");
    }
    else  //未过期
    {
        int count = ui->tableWidget->rowCount(); //该订单下的机票数
        QString tNum;
        QString sql;
        QSqlQuery query;

        QMessageBox::StandardButton result;
        result=QMessageBox::information(this,"退票信息","是否确定退票？",
                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(result==QMessageBox::Yes)
        {
            for(int i =0;i<count;i++)    //循环更新订单下所有机票的状态
            {
                tNum = ui->tableWidget->item(i,0)->text();  //机票号
                sql = QString("update ticket set ticket_status = 1 where ticketnum = '%1'").arg(tNum);
                query.exec(sql);
            }
            QMessageBox::information(this,"退票信息",tr("退票成功!退还金额%1").arg(total_money));
            orderStatus = "已失败";
        }
    }
}

void orderInfo::on_pushButton_3_clicked()   //返回按钮
{
    myOrder *order_widget = new myOrder;
    order_widget->uid = uid;   //传递用户名给order_widget
    order_widget->setInfo();  //将用户名下所有订单显示在tablewidget中
    this->close();
    order_widget->show();
}
