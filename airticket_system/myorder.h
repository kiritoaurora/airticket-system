#ifndef MYORDER_H
#define MYORDER_H

#include <QWidget>

namespace Ui {
class myOrder;
}

class myOrder : public QWidget
{
    Q_OBJECT

public:
    explicit myOrder(QWidget *parent = nullptr);
    ~myOrder();
    void setInfo();
    QString uid;
    int flag = 0;
    QString orderStatus;
private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::myOrder *ui;
};

#endif // MYORDER_H
