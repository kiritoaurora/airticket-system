#include "admininterface.h"
#include "ui_admininterface.h"
#include "widget.h"
#include "ui_widget.h"
#include "adminsearch.h"
#include "ui_adminsearch.h"
#include "updateflightinfo.h"
#include "ui_updateflightinfo.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

AdminInterface::AdminInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminInterface)
{
    ui->setupUi(this);
    this->setWindowTitle("查询界面");
    //设置tableWidget列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setVisible(true);
    //设置tableWidget_2列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget_2->hide();
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget_2->verticalHeader()->hide();
    ui->tableWidget_2->horizontalHeader()->setVisible(true);
    //设置tableWidget_3列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget_3->verticalHeader()->hide();
    ui->tableWidget_3->horizontalHeader()->setVisible(true);
    //设置tableWidget_4列宽自适应长度，列名背景色，隐藏行号
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_4->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(71, 72, 76);}");
    ui->tableWidget_4->verticalHeader()->hide();
    ui->tableWidget_4->horizontalHeader()->setVisible(true);

    QDateTime currentDate = QDateTime::currentDateTime();
    QString year = currentDate.toString("yyyy");
    QString month = currentDate.toString("MM");
    QString day = currentDate.toString("dd");
    ui->comboBox_2->setCurrentText(year);
    ui->comboBox_3->setCurrentText(month);
    ui->comboBox_4->setCurrentText(day);

    ui->radioButton_2->setChecked(1);
    ui->comboBox_4->hide();
    ui->label_18->hide();
    ui->pushButton_14->move(260,30);

    ui->widget_2->hide();
    ui->widget_3->hide();
}

AdminInterface::~AdminInterface()
{
    delete ui;
}

void AdminInterface::on_pushButton_clicked()  //查询所有航班信息按钮
{
    ui->tableWidget->show();
    ui->widget_2->hide();
    ui->widget_3->hide();
    ui->tableWidget_2->hide();
    ui->tableWidget->setRowCount(0);
    QString sql=QString("select * from adminFlightInfo");
    QSqlQuery query;
    query.exec(sql);
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
}

void AdminInterface::on_pushButton_2_clicked()   //查询特定航班信息按钮
{
    ui->tableWidget->show();
    ui->widget_2->hide();
    ui->widget_3->hide();
    ui->tableWidget_2->hide();
    adminSearch *adSearch_dialog = new adminSearch;
    adSearch_dialog->show();
    QString oStation,eStation,date,airnum;
    if(adSearch_dialog->exec()==QDialog::Accepted)
    {
        ui->tableWidget->setRowCount(0);
        oStation = adSearch_dialog->oStation;
        eStation = adSearch_dialog->eStation;
        date = adSearch_dialog->date;
        airnum = adSearch_dialog->airnum;
        if(airnum != NULL)
        {
            QString sql=QString("select * from adminFlightInfo where airnum = '%1' and date = '%2'")
                               .arg(airnum).arg(date);
            QSqlQuery query;
            query.exec(sql);
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
            if(ui->tableWidget->rowCount()==0)
                QMessageBox::information(this,"查询信息","未查找到该航班！");
        }
        else if(airnum == NULL&&oStation != NULL&&eStation != NULL)
        {
            QString sql=QString("select * from adminFlightInfo where start_station like '%'+'%1'+'%'"
                                "and end_station like '%'+'%2'+'%' and date = '%3'")
                                .arg(oStation).arg(eStation).arg(date);
            QSqlQuery query;
            query.exec(sql);
            for(int i = 0;query.next();i++)
            {
                int rowcount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowcount);
                for(int j = 0;j < 9;j++)
                {
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
            }
            if(ui->tableWidget->rowCount()==0)
                QMessageBox::information(this,"查询信息","未查找到该航班！");
        }
        else
            QMessageBox::information(this,"查询信息","未查找到该航班！");
    }
}

void AdminInterface::on_pushButton_3_clicked()  //修改航班信息按钮
{
//    ui->widget_2->show();
//    ui->tableWidget->hide();
//    ui->tableWidget_2->hide();

    updateFlightInfo *update_dialog = new updateFlightInfo;
    int row = ui->tableWidget->currentRow();
    if(row != -1)
    {
        for(int i =0;i<9;i++)
            update_dialog->info[i] = ui->tableWidget->item(row,i)->text();
        update_dialog->setInfo();
        update_dialog->show();
        if(update_dialog->exec()==QDialog::Accepted)
        {
            QString airnum,date,sTime,eTime,price;
            airnum = update_dialog->info[0];
            date = update_dialog->info[1];
            sTime = update_dialog->info[5];
            eTime = update_dialog->info[6];
            price = update_dialog->info[8];
            QString sql = QString("update flight set start_time='%1' , end_time = '%2'"
                                  ", price = '%3' where airnum = '%4' and date = '%5'")
                    .arg(sTime).arg(eTime).arg(price).arg(airnum).arg(date);
            QSqlQuery query;
            query.exec(sql);
            QMessageBox::information(this,"更新信息","更新成功！");
            on_pushButton_clicked();
        }
    }
}

void AdminInterface::on_pushButton_4_clicked()  //新增航班信息按钮
{    
    ui->widget_2->show();
    ui->widget_3->hide();
    ui->tableWidget->hide();
    ui->tableWidget_2->hide();
}

void AdminInterface::on_pushButton_5_clicked()  //查询航班乘客信息按钮
{
    adminSearch *adSearch_dialog2 = new adminSearch;
    adSearch_dialog2->show();
    QString oStation,eStation,date,airnum;
    if(adSearch_dialog2->exec()==QDialog::Accepted)
    {
        ui->tableWidget_2->setRowCount(0);
        oStation = adSearch_dialog2->oStation;
        eStation = adSearch_dialog2->eStation;
        date = adSearch_dialog2->date;
        airnum = adSearch_dialog2->airnum;
        if(airnum != NULL)
        {
            QString sql=QString(" select passage.id,name,phonenum,class_level,seatnum "
                             "from passage,ticket "
                             "where passage.id = ticket.id and airnum = '%1' and date = '%2'")
                             .arg(airnum).arg(date);
            QSqlQuery query;
            query.exec(sql);
            for(int i = 0;query.next();i++)
            {
                int rowcount = ui->tableWidget_2->rowCount();
                ui->tableWidget_2->insertRow(rowcount);
                for(int j = 0;j < 5;j++)
                {
                    ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
            }
            if(ui->tableWidget_2->rowCount()==0)
                QMessageBox::information(this,"查询信息","未查找到该航班的乘客信息！");
        }
        else if(airnum == NULL&&oStation != NULL&&eStation != NULL)
        {
            QString sql=QString("select passage.id,name,phonenum,class_level,seatnum "
              "from passage,ticket,flight where passage.id = ticket.id and "
              "ticket.airnum = flight.airnum and ticket.date = flight.date and "
              "start_station like '%'+'%1'+'%'and end_station like '%'+'%2'+'%'"
              "and flight.date = '%3' ").arg(oStation).arg(eStation).arg(date);
            QSqlQuery query;
            query.exec(sql);
            for(int i = 0;query.next();i++)
            {
                int rowcount = ui->tableWidget_2->rowCount();
                ui->tableWidget_2->insertRow(rowcount);
                for(int j = 0;j < 5;j++)
                {
                    ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
            }
            if(ui->tableWidget_2->rowCount()==0)
                QMessageBox::information(this,"查询信息","未查找到该航班的乘客信息！");
        }
        else
            QMessageBox::information(this,"查询信息","未查找到该航班的乘客信息！");

        ui->tableWidget_2->show();
        ui->tableWidget->hide();
        ui->widget_2->hide();
    }
}

void AdminInterface::on_pushButton_6_clicked()  //退出登录按钮
{
    Widget *login = new Widget;
    this->close();
    login->show();
}

void AdminInterface::getInfo()   //新增航班，获取航班信息
{
    airnum = ui->lineEdit->text();
    date = ui->lineEdit_2->text();
    oStation = ui->lineEdit_3->text();
    eStation = ui->lineEdit_4->text();
    pNum = ui->lineEdit_5->text();
    sTime = ui->lineEdit_6->text();
    eTime = ui->lineEdit_7->text();
    cName = ui->comboBox->currentText();
    price = ui->lineEdit_8->text();
}

bool AdminInterface::planeJudge()  //新增航班，判断飞机编号是否有效
{
    QString sql = QString("select  * from airplane "
                          "where planenum not in (select distinct planenum from flight)");
    QString sql2 = QString("select distinct planenum from flight where airnum = '%1'").arg(airnum);
    QSqlQuery query,query2;
    query.exec(sql);
    query2.exec(sql2);
    query2.next();
    while(query.next())
    {
        if(pNum == query.value(0).toString()||pNum == query2.value(0).toString())
        {
            return true;
        }
    }
    QMessageBox::information(this,"插入信息","飞机编号无效！");
    return false;
}

bool AdminInterface::airnumJudge()  //新增航班，判断航班号是否有效
{
    QString sql = QString("select distinct airnum,start_station,end_station from flight");
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {   //航班号是否已存在
        if(airnum == query.value(0).toString())
        {   //输入的起点和终点是否与数据库对应航班的起终点相同，若不同，则航班号或起终点无效
            if(oStation!=query.value(1).toString()||eStation!=query.value(2).toString())
            {
                QMessageBox::information(this,"插入信息","航班号或起点终点无效！");
                return false;
            }
            else
                break;
        }
    }
    return true;
}

bool AdminInterface::dateJudge()  //新增航班，判断日期是否有效
{
     QDateTime currentDate = QDateTime::currentDateTime();
     QString nowDate = currentDate.toString("yyyy-MM-dd");
     if(date<nowDate)     //航班日期必须大于当前日期
     {
         QMessageBox::information(this,"插入信息","日期无效！");
         return false;
     }
     else
         return true;
}

bool AdminInterface::isFull()  //新增航班，判断信息是否已填完全
{
    if(airnum.length() == 2||date == NULL||oStation == NULL||eStation == NULL||pNum == NULL
            ||sTime == NULL||eTime == NULL||cName == NULL||price == NULL)
    {
        QMessageBox::information(this,"插入信息","部分信息为空！");
        return false;
    }
    else
        return true;
}

bool AdminInterface::newFlightInfoJudge()  //判定输入的信息是否有效
{
    getInfo();   //获取输入的信息
    bool res1,res2,res3,res4;
    res1 = isFull();   //判定信息是否填全
    if(res1 == true)
    {
        res2 = airnumJudge();   //判定航班号是否有效
        if(res2 == true)
        {
            res3 = dateJudge();   //判定日期是否有效
            if(res3 == true)
                res4 = planeJudge();  //判定飞机编号是否有效
        }
    }

    if(res1 == true && res2 ==true && res3 ==true && res4 ==true)  //全部有效时，返回true
    {
        return true;
    }
    else
        return false;
}

bool AdminInterface::isNotSame()   //判定输入的航班是否已存在
{
    QString sql = QString("select airnum,date from flight where airnum = '%1' and date = '%2'")
            .arg(airnum).arg(date);
    QSqlQuery query;
    query.exec(sql);
    if(query.numRowsAffected()==0)
        return true;
    else
        return false;
}

void AdminInterface::on_pushButton_7_clicked()  //新增航班 确认按钮
{
//    QString airnum,date,oStation,eStation,pNum,sTime,eTime,cName,price;
    bool res,res1;
    res = newFlightInfoJudge();  //判定输入的全部信息是否有效
    res1 = isNotSame();     //判定输入的航班信息是否已存在
    if(res == true)
    {
        if(res1 == true)
        {
            QMessageBox::information(this,"插入信息","新增航班成功！");
            QString sql = QString("insert into flight values('%1','%2','%3','%4','%5','%6','%7','%8',%9)")
                    .arg(airnum).arg(date).arg(oStation).arg(eStation).arg(pNum)
                    .arg(sTime).arg(eTime).arg(cName).arg(price);
            QSqlQuery query;
            query.exec(sql);
        }
        else
            QMessageBox::information(this,"插入信息","该航班已存在！");
    }
}

void AdminInterface::on_pushButton_8_clicked()  //新增航班 取消按钮
{
    ui->tableWidget->show();
    ui->widget_2->hide();
    ui->widget_3->hide();
}

void AdminInterface::on_pushButton_9_clicked()  //新增航班 查询闲置飞机按钮
{
    ui->tableWidget_3->setRowCount(0);
    QString sql = QString("select  * from airplane "
                          "where planenum not in (select distinct planenum from flight)");
    QSqlQuery query;
    query.exec(sql);
    for(int i = 0;query.next();i++)
    {
        int rowcount = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rowcount);
        for(int j = 0;j < 4;j++)
        {
            ui->tableWidget_3->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget_3->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }

}

void AdminInterface::on_pushButton_12_clicked()  //新增航班 删除闲置飞机按钮
{
    int row = ui->tableWidget_3->currentRow();
    if(row != -1)
    {
        QMessageBox::StandardButton result;
        result=QMessageBox::information(this,"删除信息","是否确定删除该飞机信息？",
                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(result==QMessageBox::Yes)
        {
            QString pNum = ui->tableWidget_3->item(row,0)->text();
            QString sql = QString("delete airplane where planenum = '%1'").arg(pNum);
            QSqlQuery query;
            query.exec(sql);
            on_pushButton_9_clicked();
        }
    }
}

void AdminInterface::on_pushButton_10_clicked()  //新增航班 生成飞机编号按钮
{
    QString sql = QString("select top 1 planenum from airplane order by planenum desc");
    QSqlQuery query;
    query.exec(sql);
    query.next();
    int pNum = query.value(0).toInt();
    pNum += 1;
    ui->lineEdit_9->setText(QString::number(pNum));
}

void AdminInterface::on_pushButton_11_clicked()     //新增航班 添加新飞机信息按钮
{
    QString pNum,pType,fClass,sClass;
    pNum = ui->lineEdit_9->text();
    pType = ui->lineEdit_10->text();
    fClass = ui->lineEdit_11->text();
    sClass = ui->lineEdit_12->text();
    if(pNum != NULL&&pType != NULL&&fClass != NULL&&sClass != NULL)
    {
        int rowcount = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rowcount);
        ui->tableWidget_3->setItem(rowcount,0,new QTableWidgetItem(pNum));
        ui->tableWidget_3->setItem(rowcount,1,new QTableWidgetItem(pType));
        ui->tableWidget_3->setItem(rowcount,2,new QTableWidgetItem(fClass));
        ui->tableWidget_3->setItem(rowcount,3,new QTableWidgetItem(sClass));
        for(int j = 0;j < 4;j++)
        {
            ui->tableWidget_3->item(rowcount,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        ui->lineEdit_9->setText("");
        ui->lineEdit_10->setText("");
        ui->lineEdit_11->setText("");
        ui->lineEdit_12->setText("");
        QString sql = QString("insert into airplane values('%1','%2',%3,%4)").arg(pNum)
                .arg(pType).arg(fClass).arg(sClass);
        QSqlQuery query;
        query.exec(sql);
    }
    else
        QMessageBox::information(this,"插入信息","部分信息为空！");
}



void AdminInterface::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "中国国航")
        ui->lineEdit->setText("CA");
    else if(arg1 == "南方航空")
        ui->lineEdit->setText("CZ");
    else if(arg1 == "东方航空")
        ui->lineEdit->setText("MU");
}

void AdminInterface::on_pushButton_13_clicked()  //销售汇总按钮
{
    ui->widget_3->show();
    ui->widget_2->hide();
    ui->tableWidget->hide();
    ui->tableWidget_2->hide();
}

void AdminInterface::on_pushButton_14_clicked()    //销售汇总、查询按钮
{
    ui->tableWidget_4->setRowCount(0);
    QString sql;
    QSqlQuery query;
    if(ui->radioButton_2->isChecked())
    {
        QString year,month;
        year = ui->comboBox_2->currentText();
        month = ui->comboBox_3->currentText();
        QString searchDate = year+"-"+month;   // 月份
        sql = QString("select airnum,date,start_station,end_station,comp_name,sum(销售数量) 销量 from soleAmount "
                              "where orderDate like '%'+'%1'+'%'"
                              "group by airnum,date,start_station,end_station,comp_name").arg(searchDate);

    }
    else
    {
        QString year,month,day;
        year = ui->comboBox_2->currentText();
        month = ui->comboBox_3->currentText();
        day = ui->comboBox_4->currentText();
        QString searchDate = year+"-"+month+'-'+day;   // 日期
        sql = QString("select airnum,date,start_station,end_station,comp_name,sum(销售数量) 销量 from soleAmount "
                              "where orderDate = '%1'"
                              "group by airnum,date,start_station,end_station,comp_name").arg(searchDate);
    }

    query.exec(sql);
    for(int i = 0;query.next();i++)
    {
        int rowcount = ui->tableWidget_4->rowCount();
        ui->tableWidget_4->insertRow(rowcount);
        for(int j = 0;j < 6;j++)
        {
            ui->tableWidget_4->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            ui->tableWidget_4->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}

void AdminInterface::on_radioButton_clicked()  //按日期查询 选择按钮
{
    ui->comboBox_4->show();
    ui->label_18->show();
    ui->pushButton_14->move(390,30);
}

void AdminInterface::on_radioButton_2_clicked()  //按年月查询 选择按钮
{
    ui->comboBox_4->hide();
    ui->label_18->hide();
    ui->pushButton_14->move(260,30);
}
