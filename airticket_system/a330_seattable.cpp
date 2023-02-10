#include "a330_seattable.h"
#include "ui_a330_seattable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

A330_seatTable::A330_seatTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::A330_seatTable)
{
    ui->setupUi(this);
    this->setWindowTitle("值机界面");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setMouseTracking(true);
}

A330_seatTable::~A330_seatTable()
{
    delete ui;
}

void A330_seatTable::createArray()
{
    X = new int[amount];
    Y = new int[amount];
    seatnum = new QString[amount];

}

void A330_seatTable::exchange(QString seatnum)
{
    QString i = seatnum.mid(0,1);
    QString j = seatnum.mid(1,1);

    a = i.toInt()-1;

    if(j == "A")
        b = 0;
    else if(j == "B")
        b = 1;
    else if(j == "C")
        b = 3;
    else if(j == "D")
        b = 4;
    else if(j == "E")
        b = 6;
    else if(j == "F")
        b = 7;
}

void A330_seatTable::fillTable()
{
    for(int i =0;i<5;i++)
    {
        seat_table[i][2] = 3;
        seat_table[i][5] = 3;
    }
    QString sql = QString("select seatnum from ticket where airnum = '%1' and date = '%2'"
                          "and class_level = '%3' and ticket_status = 0").arg(airnum).arg(date).arg(class_level);
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
        if(query.value(0).toString() != NULL)
        {
            exchange(query.value(0).toString());
            seat_table[a][b] = 1;
        }
    }

}

void A330_seatTable::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;

    //绘制座椅
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(seat_table[i][j]==0)
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::white);
                painter.setPen(pen);
                painter.drawRect((j+1)*60-30,(i+1)*80-30,50,50);
            }
            if(seat_table[i][j]==1)
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::red);
                painter.setPen(pen);
                painter.drawRect((j+1)*60-30,(i+1)*80-30,50,50);
            }
            if(seat_table[i][j]==2)
            {
                pen.setColor(Qt::black);
                painter.setBrush(Qt::green);
                painter.setPen(pen);
                painter.drawRect((j+1)*60-30,(i+1)*80-30,50,50);
            }

        }
    }

}

void A330_seatTable::mouseMoveEvent(QMouseEvent * event)
{
    moveX=event->x();
    moveY=event->y();
    int i,j;
    for(i=0;i<5;i++)       //判断离鼠标最近的点
    {
        for(j=0;j<8;j++)
        {
            if(moveX-(j+1)*60<15&&moveY-(i+1)*80<15)
            {
                a=i;
                b=j;
                break;
            }
        }
        if(a==i&&b==j)
            break;
    }

}

void A330_seatTable::mousePressEvent(QMouseEvent *)
{
    if(seat_table[a][b] == 0)
    {
        seat_table[a][b] = 2;
        if(count != amount)
        {
            X[count] = a;
            Y[count] = b;
            count++;
        }
        else
        {
            int row,col;
            row = X[0];
            col = Y[0];
            seat_table[row][col] = 0;
            for(int i =1;i<amount;i++)
            {
                X[i-1] = X[i];
                Y[i-1] = Y[i];
            }
            X[amount-1] = a;
            Y[amount-1] = b;
        }
        update();
    }
    else if(seat_table[a][b] == 2)
    {
        seat_table[a][b] = 0;
        for(int i =0;i<count;i++)
        {
            if(X[i]==a&&Y[i]==b)
            {
                if(i == count-1)
                    count--;
                else
                {
                    for(int j = i+1 ;j<count;j++)
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
QString A330_seatTable::numToChar(int num)
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
    else if(num == 6)
        c = "E";
    else if(num == 7)
        c = "F";

    return c;
}

void A330_seatTable::on_pushButton_2_clicked()  //取消按钮
{
    this->close();
}

void A330_seatTable::on_pushButton_clicked()   //确认按钮
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
            row = QString::number(X[i]+1);
            col = numToChar(Y[i]);
            seat = row + col;
            qDebug()<<seat;
            seatnum[i] = seat;
        }
        QMessageBox::StandardButton result;
        result=QMessageBox::information(this,"值机信息","是否确定该选座？",
                     QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(result==QMessageBox::Yes)
            accept();
    }
}
