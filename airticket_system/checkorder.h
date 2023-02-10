#ifndef CHECKORDER_H
#define CHECKORDER_H

#include <QDialog>

namespace Ui {
class checkOrder;
}

class checkOrder : public QDialog
{
    Q_OBJECT

public:
    explicit checkOrder(QWidget *parent = nullptr);
    ~checkOrder();
    void setInfo();
    QString ordernum,airnum,oStation,eStation,date,sTime,eTime,cName,total_money,total_passages;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::checkOrder *ui;
};

#endif // CHECKORDER_H
