#ifndef BOOK_TICKET_H
#define BOOK_TICKET_H

#include <QWidget>

namespace Ui {
class book_ticket;
}

class book_ticket : public QWidget
{
    Q_OBJECT

public:
    explicit book_ticket(QWidget *parent = nullptr);
    ~book_ticket();
    void setInfo();
    void total_money(float price);
    QString CreateTicketNum();
    QString CreateOrderNum();
    void takeOrder(int amount);
    QString uid;
    QString ordernum;
    QString airnum,oStation,eStation,date,sTime,eTime,cName,fprice,sprice;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_tableWidget_2_cellDoubleClicked(int row, int column);

private:
    Ui::book_ticket *ui;
};

#endif // BOOK_TICKET_H
