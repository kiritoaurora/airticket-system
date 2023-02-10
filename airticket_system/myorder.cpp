#include "myorder.h"
#include "ui_myorder.h"
#include "orderinfo.h"
#include "ui_orderinfo.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
myOrder::myOrder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myOrder)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
    this->setWindowTitle("订单界面");
    //设置tableWidget列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget->verticalHeader()->hide();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString year = currentDate.toString("yyyy");
    QString month = currentDate.toString("MM");
    ui->comboBox->setCurrentText(year);
    ui->comboBox_2->setCurrentText(month);
}

myOrder::~myOrder()
{
    delete ui;
}

void myOrder::setInfo()  //根据登录的账户将其所有的订单显示在tablewidget中
{
    QDateTime date = QDateTime::currentDateTime();
    QString currentTime = date.currentDateTime().toString("hh:mm:ss");
    QString currentDate = date.currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery query;
    QString sql = QString("select * from orderInformation where username = '%1'").arg(uid);
    query.exec(sql);
    //根据订单数设置tableWidget的行数，并按日期降序居中输出显示
    ui->tableWidget->setRowCount(query.numRowsAffected());
    for(int i = query.numRowsAffected()-1;query.next();i--)
    {
        for(int j = 0;j < 7;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        QString ordernum = query.value(0).toString();
        QString sql2 = QString("select seatnum from ticket where ordernum = '%1'").arg(ordernum);
        QSqlQuery query2;
        query2.exec(sql2);
        query2.next();
        if(query.value(7).toString() == "1")
        {
            ui->tableWidget->setItem(i,7,new QTableWidgetItem("已失败"));
            ui->tableWidget->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }    //query2.value(0).toString() != NULL&&
        else if(currentDate > query.value(3).toString()||(currentDate == query.value(3).toString()&&currentTime > query.value(8).toString()))
        {
            ui->tableWidget->setItem(i,7,new QTableWidgetItem("已出行"));
            ui->tableWidget->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }    //query2.value(0).toString() == NULL&&
        else if(currentDate < query.value(3).toString()||(currentDate == query.value(3).toString()&&currentTime < query.value(8).toString()))
        {
            ui->tableWidget->setItem(i,7,new QTableWidgetItem("未出行"));
            ui->tableWidget->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}
/***********************************************************************/
//选中订单并双击，进入订单详情界面
/***********************************************************************/
void myOrder::on_tableWidget_cellDoubleClicked(int row, int column)
{
    column = 0;
    orderInfo *ticketInfo_widget = new orderInfo;
    //传递订单号和总金额给ticketInfo_widget
    ticketInfo_widget->ordernum = ui->tableWidget->item(row,column)->text();
    ticketInfo_widget->total_money = ui->tableWidget->item(row,6)->text();
    ticketInfo_widget->uid = uid;
    ticketInfo_widget->setInfo();  //将订单下的机票信息显示在tablewidget中
    ticketInfo_widget->orderStatus = ui->tableWidget->item(row,7)->text();
    this->close();
    ticketInfo_widget->show();   
}

void myOrder::on_pushButton_clicked()  //订单查询按钮
{
    QString year,month;
    year = ui->comboBox->currentText();
    month = ui->comboBox_2->currentText();
    QString day1 = "01",day2 = "31";
    QString searchDate = year+"-"+month+"-"+day1;   // 月初日期
    QString searchDate2 = year+"-"+month+"-"+day2;  //月末日期

//    QSqlQuery query;
//    QString sql = QString("select * from orderInformation where username = '%1'").arg(uid);
//    query.exec(sql);
//    //根据订单数设置tableWidget的行数，并按日期降序居中输出显示
//    ui->tableWidget->setRowCount(query.numRowsAffected());
//    for(int i = query.numRowsAffected()-1;query.next();i--)
//    {
//        for(int j = 0;j < 7;j++)
//        {
//            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
//            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        }
//        if(!(query.value(4).toString()>searchDate&&query.value(4).toString()<searchDate2))
//        {
//            ui->tableWidget->removeRow(i);
//        }
//    }

    if(flag == 0)
    {
        ui->tableWidget->setRowCount(0);
        setInfo();
        int rowCount = ui->tableWidget->rowCount();
        for(int i =0;i<rowCount;i++)
        {
            QString orderDate = ui->tableWidget->item(i,4)->text();
            if(!(orderDate > searchDate&&orderDate < searchDate2))
            {
                ui->tableWidget->removeRow(i);
                rowCount = ui->tableWidget->rowCount();
                i = 0;
            }
        }
        if((rowCount == 1||rowCount == 2) && (ui->tableWidget->item(0,4)->text() < searchDate||ui->tableWidget->item(0,4)->text() > searchDate2))
        {
            ui->tableWidget->removeRow(0);
        }
    }
    else if(flag == 1)
    {
        if(orderStatus == "未出行")
        {
            on_pushButton_3_clicked();
        }
        else if(orderStatus == "已出行")
        {
            on_pushButton_4_clicked();
        }
        else if(orderStatus == "已失败")
        {
            on_pushButton_5_clicked();
        }
        int rowCount = ui->tableWidget->rowCount();
        for(int i =0;i<rowCount;i++)
        {
            QString orderDate = ui->tableWidget->item(i,4)->text();
            //qDebug()<<i<<orderDate<<searchDate<<searchDate2<<endl;
            if(!(orderDate > searchDate&&orderDate < searchDate2))
            {
                ui->tableWidget->removeRow(i);
                rowCount = ui->tableWidget->rowCount();
                //qDebug()<<rowCount;
                i = 0;
            }
        }
        if((rowCount == 1||rowCount == 2) && (ui->tableWidget->item(0,4)->text() < searchDate||ui->tableWidget->item(0,4)->text() > searchDate2))
        {   //qDebug()<<ui->tableWidget->item(0,4)->text()<<searchDate<<searchDate2;
            ui->tableWidget->removeRow(0);
        }
    }
}

void myOrder::on_pushButton_2_clicked()  //查询所有订单信息
{
    setInfo();
    flag = 0;
}

void myOrder::on_pushButton_3_clicked()  //未出行 按钮
{
    ui->tableWidget->setRowCount(0);
    setInfo();
    flag = 1;
    orderStatus = "未出行";
    int rowCount = ui->tableWidget->rowCount();
    for(int i =0;i<rowCount;i++)
    {
        QString status = ui->tableWidget->item(i,7)->text();
        if(status != "未出行")
        {
            ui->tableWidget->removeRow(i);
            rowCount = ui->tableWidget->rowCount();
            i = 0;
        }
    }
    if(ui->tableWidget->item(0,7)->text() != "未出行")
    {
        ui->tableWidget->removeRow(0);
    }
}

void myOrder::on_pushButton_4_clicked()   //已出行  按钮
{
    ui->tableWidget->setRowCount(0);
    setInfo();
    flag = 1;
    orderStatus = "已出行";
    int rowCount = ui->tableWidget->rowCount();
    for(int i =0;i<rowCount;i++)
    {   //qDebug()<<rowCount<<ui->tableWidget->item(i,7)->text()<<endl;
        if(ui->tableWidget->item(i,7)->text() != "已出行")
        {
            ui->tableWidget->removeRow(i);
            rowCount = ui->tableWidget->rowCount();
            i = 0;
        }
        //qDebug()<<rowCount<<ui->tableWidget->item(i,7)->text();
    }
    if(ui->tableWidget->item(0,7)->text() != "已出行")
    {
        ui->tableWidget->removeRow(0);
    }
}

void myOrder::on_pushButton_5_clicked()   //已失败  按钮
{
    ui->tableWidget->setRowCount(0);
    setInfo();
    flag = 1;
    orderStatus = "已失败";
    int rowCount = ui->tableWidget->rowCount();
    for(int i =0;i<rowCount;i++)
    {
        if(ui->tableWidget->item(i,7)->text() != "已失败")
        {
            ui->tableWidget->removeRow(i);
            rowCount = ui->tableWidget->rowCount();
            i = 0;
        }
    }
    if(ui->tableWidget->item(0,7)->text() != "已失败")
    {
        ui->tableWidget->removeRow(0);
    }
}
