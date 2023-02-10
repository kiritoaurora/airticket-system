#include "book_ticket.h"
#include "ui_book_ticket.h"
#include "passages.h"
#include "ui_passages.h"
#include "checkorder.h"
#include "ui_checkorder.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

book_ticket::book_ticket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::book_ticket)
{
    ui->setupUi(this);
    this->setWindowTitle("订票界面");
    //设置tableWidget列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget->verticalHeader()->hide();
    ui->radioButton_2->setChecked(true);
    //设置tableWidget_2列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget_2->verticalHeader()->hide();
    ui->radioButton_2->setChecked(true);
}

book_ticket::~book_ticket()
{
    delete ui;
}

void book_ticket::setInfo()
{
    ui->lineEdit->setText(airnum);
    ui->lineEdit_2->setText(oStation);
    ui->lineEdit_3->setText(eStation);
    ui->lineEdit_4->setText(date);
    ui->lineEdit_5->setText(sTime);
    ui->lineEdit_6->setText(eTime);
    ui->lineEdit_7->setText(cName);
    ui->lineEdit_8->setText(sprice);
    QString sql = QString("exec oldPassagers '%1'").arg(uid);
    QSqlQuery query;
    query.exec(sql);
    for(int i =0;query.next();i++)
    {
        int rowCount = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(rowCount);
        for(int j =0;j<3;j++)
        {
            ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

void book_ticket::total_money(float price)  //根据乘机人数量和舱位选择计算出总金额
{
    int amount = ui->tableWidget->rowCount();  //记录有几名乘机人
    float sum = 0.0;   //总金额
    QString id;        //乘机人id
    for(int i = 0;i<amount;i++)
    {
        id = ui->tableWidget->item(i,0)->text();
        QString sql = QString("select mlevel from member where id = '%1' and comp_name = '%2'")
                      .arg(id).arg(cName);  //查询乘机人的会员等级
        QSqlQuery query;
        query.exec(sql);
        if(query.next()==false||query.value(0).toString() == "普通会员")  //会员表中无乘机人信息，按普通会员计算价格
        {
            sum += price;
        }
        else if(query.value(0).toString() == "银卡会员")  //银卡会员机票打95折
        {
            sum += price*0.95;
        }
        else if(query.value(0).toString() == "金卡会员")  //金卡会员机票打85折
        {
            sum += price*0.85;
        }
    }
    QString sumMoney = QString::number(sum,'f',2);
    ui->lineEdit_9->setText(sumMoney);
}
/****************************************************************************/
//机票编号为当前日期的 month+day+数据库中形如month+day的信息条数+1+10000
//即：MMdd1XXXX
/****************************************************************************/
QString book_ticket::CreateTicketNum()    //生成机票编号
{
    QDateTime currentDate = QDateTime::currentDateTime();  //获取当前系统日期
    QString month = currentDate.toString("MM");  //获取月份
    QString day = currentDate.toString("dd");   //获取号数
    QString num = month + day;   //获取机票编号前缀
    QString sql = QString("select count(*) from ticket where ticketnum like '%1'+'%'")
            .arg(num);    //查询形如“MMdd”的数据条数
    QSqlQuery query;
    query.exec(sql);
    query.next();
    int amount = query.value(0).toInt() + 1 + 10000;  //获取机票编号后缀
    QString tNum = num + QString::number(amount);
    return tNum;
}
/****************************************************************************/
//订单编号为订单条数+1+100000
//即：1XXXXX
/****************************************************************************/
QString book_ticket::CreateOrderNum()  //生成订单号
{
    QString sql = QString("select count(*) from orderform");  //查询订单条数
    QSqlQuery query;
    query.exec(sql);
    query.next();
    int amount = query.value(0).toInt() + 1 + 100000;  //获取订单编号
    QString oNum = QString::number(amount);
    return oNum;
}

void book_ticket::takeOrder(int amount)  //确认下单后，将相应数据存入数据库
{
    QString sql;
    QSqlQuery query;
    QString id,name,pnum,class_level,total_money;
    //插入订单信息,ordernum作为全局变量，点击下单按钮时生成，此处可直接用
    total_money = ui->lineEdit_9->text();
    QDateTime currentDate = QDateTime::currentDateTime();
    QString nowDate = currentDate.toString("yyyy-MM-dd");
    //插入订单信息
    sql = QString("exec newOrder '%1','%2','%3',%4")
            .arg(ordernum).arg(nowDate).arg(uid).arg(total_money);
    query.exec(sql);
    //判断舱位等级
    if(ui->radioButton->isChecked() == 1)
        class_level = "商务舱";
    else
        class_level = "经济舱";

    for(int i = 0;i<amount;i++)
    {
        id = ui->tableWidget->item(i,0)->text();
        name = ui->tableWidget->item(i,1)->text();
        pnum = ui->tableWidget->item(i,2)->text();
        //插入乘客信息
        sql = QString("exec newPassage '%1','%2','%3'").arg(id).arg(name).arg(pnum);
        QSqlQuery query;
        query.exec(sql);
        //插入会员信息
        sql = QString("exec newMember '%1','%2','%3'").arg(id).arg(airnum).arg(date);
        query.exec(sql);
        //插入机票信息
        QString tNum = CreateTicketNum();
        sql = QString("exec newTicket '%1','%2','%3','%4','%5','%6'")
                .arg(tNum).arg(airnum).arg(date).arg(class_level).arg(id).arg(ordernum);
        query.exec(sql);
    }
}

void book_ticket::on_pushButton_clicked() //添加乘机人按钮
{
    Passages *passage_widget = new Passages;
    passage_widget->show();
    float price;   //用于获取机票价格
    if(passage_widget->exec()==QDialog::Accepted)
    {
        QString id,name,pnum;
        id = passage_widget->id;
        name = passage_widget->name;
        pnum = passage_widget->pnum;
        passage_widget->close();
        //将乘机人的信息显示在tablewidget中
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        ui->tableWidget->setItem(rowcount,0,new QTableWidgetItem(id));
        ui->tableWidget->setItem(rowcount,1,new QTableWidgetItem(name));
        ui->tableWidget->setItem(rowcount,2,new QTableWidgetItem(pnum));
        //tablewidget中的数据居中显示
        ui->tableWidget->item(rowcount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->item(rowcount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->item(rowcount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        if(ui->radioButton->isChecked() == 1)  //选择商务舱
            price= fprice.toFloat();   //商务舱价格
        else
            price = sprice.toFloat();  //经济舱价格
        total_money(price);   //计算订单总金额
    }
}

void book_ticket::on_pushButton_2_clicked()  //立即下单按钮
{
    int amount = ui->tableWidget->rowCount();  //记录有几名乘机人
    if(amount == 0)
    {
        QMessageBox::information(this,"下单信息","未添加乘机人！");
    }
    else
    {
        ordernum = CreateOrderNum();  //生成订单号
        checkOrder *check_widget = new checkOrder;
        //将订单信息传递给check_widget用于显示
        check_widget->ordernum = ordernum;
        check_widget->airnum = airnum;
        check_widget->oStation = oStation;
        check_widget->eStation = eStation;
        check_widget->sTime = sTime;
        check_widget->eTime = eTime;
        check_widget->date = date;
        check_widget->cName = cName;
        check_widget->total_money = ui->lineEdit_9->text();
        check_widget->total_passages = QString::number(amount);
        check_widget->setInfo();  //将获得的信息显示到check_widget中
        check_widget->show();   //显示窗口
        if(check_widget->exec()==QDialog::Accepted)
        {
            takeOrder(amount);  //下单成功，将乘客信息，机票信息，订单信息以及会员信息存入数据库
            QMessageBox::StandardButton result;
            result=QMessageBox::information(this,"下单信息","订票成功！",
                         QMessageBox::Yes);
            if(result==QMessageBox::Yes)
                this->close();
        }
    }

}

void book_ticket::on_pushButton_3_clicked() //移除乘机人按钮
{
    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
    float price;
    if(ui->radioButton->isChecked() == 1)
        price= fprice.toFloat();
    else
        price = sprice.toFloat();
    total_money(price);
}

void book_ticket::on_radioButton_clicked()   //商务舱按钮
{
    ui->lineEdit_8->setText(fprice);
    float price = fprice.toFloat();
    total_money(price);
}

void book_ticket::on_radioButton_2_clicked()   //经济舱按钮
{
    ui->lineEdit_8->setText(sprice);
    float price = sprice.toFloat();
    total_money(price);
}

void book_ticket::on_tableWidget_2_cellDoubleClicked(int row, int column) //双击过往乘机人信息，选择乘机人
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    QString info[3];   //存放选中的乘客信息
    for(column = 0;column<3;column++)
    {
        info[column]=ui->tableWidget_2->item(row,column)->text();
    }
    for(int j =0;j<3;j++)
    {
        ui->tableWidget->setItem(rowCount,j,new QTableWidgetItem(info[j]));
        ui->tableWidget->item(rowCount,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    float price;   //用于获取机票价格
    if(ui->radioButton->isChecked() == 1)  //选择商务舱
        price= fprice.toFloat();   //商务舱价格
    else
        price = sprice.toFloat();  //经济舱价格
    total_money(price);   //计算订单总金额
}
