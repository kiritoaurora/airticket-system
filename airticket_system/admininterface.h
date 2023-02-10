#ifndef ADMININTERFACE_H
#define ADMININTERFACE_H

#include <QWidget>

namespace Ui {
class AdminInterface;
}

class AdminInterface : public QWidget
{
    Q_OBJECT

public:
    explicit AdminInterface(QWidget *parent = nullptr);
    ~AdminInterface();
    void getInfo();
    bool isFull();
    bool isNotSame();
    bool newFlightInfoJudge();
    bool planeJudge();
    bool airnumJudge();
    bool dateJudge();
    QString airnum,date,oStation,eStation,pNum,sTime,eTime,cName,price;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::AdminInterface *ui;
};

#endif // ADMININTERFACE_H
