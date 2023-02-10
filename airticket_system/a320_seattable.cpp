#include "a320_seattable.h"
#include "ui_a320_seattable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

A320_seatTable::A320_seatTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::A320_seatTable)
{
    ui->setupUi(this);
    this->setWindowTitle("值机界面");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setMouseTracking(true);
}

A320_seatTable::~A320_seatTable()
{
    delete ui;
}
//根据机票数创建对应长度的数组用于存放座位在数组中的下标
void A320_seatTable::createArray()
{
    X = new int[amount];  //行
    Y = new int[amount];   //列
    seatnum = new QString[amount];  //用于存放确定好的座位号
}
//将数据库中的座位号转化为座位二维数组中的下标
void A320_seatTable::exchange(QString seatnum)
{
    QString i = seatnum.mid(0,1);  //获取字符串seatnum的子串，获取第一个字符，即：行号
    QString j = seatnum.mid(1,1);  //获取字符串seatnum的子串，获取第二个字符，即：列号

    a = i.toInt()-1;   //列号-1，与数组下标对齐
    //将列号ABCD转为0，1，2，3
    if(j == "A")
        b = 0;
    else if(j == "B")
        b = 1;
    else if(j == "C")
        b = 3;
    else if(j == "D")
        b = 4;
}
/********************************************************/
//扫描对应航班和日期的机票信息，并将已有座位号转化为对应下标，
//并将seat_table二维数组中对应下标的数置为1
/********************************************************/
void A320_seatTable::fillTable()
{
    seat_table[0][2] = 3;       //第二列不画座位
    seat_table[1][2] = 3;
    QString sql = QString("select seatnum from ticket where airnum = '%1' and date = '%2'"
                     "and class_level = '%3' and ticket_status = 0").arg(airnum).arg(date).arg(class_level);
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
        if(query.value(0).toString() != NULL)
        {
            exchange(query.value(0).toString());
            seat_table[a][b] = 1;   //已选座位设置为1
        }
    }

}
/********************************************************/
//绘画事件，根据二维数组seat_table绘制座位
// 1为已选状态--红色 ， 2为正在选中状态--绿色
// 0为未选中状态--白色 ， 3为不画座位
/********************************************************/
void A320_seatTable::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;

    //绘制座椅，即：绘制一个60x60的正方形，初始坐标为40，70，循环绘制
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(seat_table[i][j]==0)   //0为空座位，填充颜色为白色
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::white);
                painter.setPen(pen);
                painter.drawRect((j+1)*70-30,(i+1)*100-30,60,60);
            }
            if(seat_table[i][j]==1)   //1为空座位，填充颜色为红色
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawRect((j+1)*70-30,(i+1)*100-30,60,60);
            }
            if(seat_table[i][j]==2)   //2为空座位，填充颜色为绿色
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::green);
                painter.setPen(pen);
                painter.drawRect((j+1)*70-30,(i+1)*100-30,60,60);
            }

        }
    }

}
/******************************************************************/
//鼠标移动事件，根据鼠标坐标计算出离鼠标最近的绘画点，同时也是seat_table的下标
/******************************************************************/
void A320_seatTable::mouseMoveEvent(QMouseEvent * event)
{
    moveX=event->x();
    moveY=event->y();
    int i,j;
    for(i=0;i<2;i++)       //判断离鼠标最近的点
    {
        for(j=0;j<5;j++)
        {
            if(moveX-(j+1)*70<30&&moveY-(i+1)*100<30)  //鼠标当前坐标与座位中心点坐标的距离小于30
            {                                         //记录下该座位的对应的下标
                a=i;
                b=j;
                break;
            }
        }
        if(a==i&&b==j)
            break;
    }

}
/********************************************************/
//鼠标点击事件，根据鼠标移动事件计算出的下标判定该下标对应的内容
//若为0，则置为2，并重绘界面
/********************************************************/
void A320_seatTable::mousePressEvent(QMouseEvent *)
{
    if(seat_table[a][b] == 0)   //该座位为未选中状态
    {
        seat_table[a][b] = 2;
        if(count != amount) //如果选座数未满，则将对应下标存入数组中记录
        {
            X[count] = a;
            Y[count] = b;
            count++;
        }
        else       //选座数已满，则将第一个座位的下标清除，并将后续下标前移，将新下标存入数组最后
        {
            int row,col;
            row = X[0];      //获取第一个已选座位的下标
            col = Y[0];
            seat_table[row][col] = 0;   //将第一个已选座位赋值为0
            for(int i =1;i<amount;i++)  //将后续座位的下标前移
            {
                X[i-1] = X[i];
                Y[i-1] = Y[i];
            }
            X[amount-1] = a;    //新座位的下标放入数组最后
            Y[amount-1] = b;
        }
        update();    //重绘界面
    }
    else if(seat_table[a][b] == 2) //若已为选中状态，则将该下标对应内容置为0
    {
        seat_table[a][b] = 0;
        for(int i =0;i<count;i++)  //将该座位下标清除，将后续座位下标前移
        {
            if(X[i]==a&&Y[i]==b)  //查找该座位的下标
            {
                if(i == count-1)  //若该座位的下标在数组中的最后，则令count-1；
                    count--;
                else
                {
                    for(int j = i+1 ;j<count;j++) //不在最后，则将后续座位下标前移
                    {
                        X[j-1] = X[j];
                        Y[j-1] = Y[j];
                    }
                }
            }
        }
        update();
    }
}
//将下标转化为字符，对应为列号
QString A320_seatTable::numToChar(int num)
{
    QString c;
    if(num == 0)
        c = "A";
    else if(num == 1)
        c = "B";
    else if(num == 3)
        c = "C";
    else if(num == 4)
        c = "D";

    return c;
}

void A320_seatTable::on_pushButton_2_clicked()  //取消按钮
{
    this->close();
}

void A320_seatTable::on_pushButton_clicked()   //确认按钮
{
    if(count != amount)
    {
        QMessageBox::information(this,"值机信息","尚有机票未完成选座！");
    }
    else
    {
        QString row,col,seat;
        for(int i =0;i<amount;i++)
        {
            row = QString::number(X[i]+1);   //获取行号
            col = numToChar(Y[i]);    //获取列号
            seat = row + col;   //合并为座位号
            qDebug()<<seat;
            seatnum[i] = seat;  //存入数组
        }
        QMessageBox::StandardButton result;
        result=QMessageBox::information(this,"值机信息","是否确定该选座？",
                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(result==QMessageBox::Yes)
            accept();
    }
}
