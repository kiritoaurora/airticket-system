#ifndef ORDERINFO_H
#define ORDERINFO_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class orderInfo;
}

class orderInfo : public QWidget
{
    Q_OBJECT

public:
    explicit orderInfo(QWidget *parent = nullptr);
    ~orderInfo();
    void setInfo();
    void saveSeatNum(QString ticketNum,QString seatNum);//将定下的座位号存入数据库

    void A320_firstLevel();   //机型为空客320的商务舱选座函数
    void A320_secondLevel();    //机型为空客320的经济舱选座函数

    void A330_firstLevel();   //机型为空客330的商务舱选座函数
    void A330_secondLevel();    //机型为空客330的经济舱选座函数

    void B737_firstLevel();   //机型为波音737的商务舱选座函数
    void B737_secondLevel();   //机型为波音737的商务舱选座函数
    QString uid;
    QString ordernum,total_money;
    QString orderStatus;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::orderInfo *ui;
};

#endif // ORDERINFO_H
