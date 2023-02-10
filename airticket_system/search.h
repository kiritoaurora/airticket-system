#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class search;
}

class search : public QWidget
{
    Q_OBJECT

public:
    explicit search(QWidget *parent = nullptr);
    ~search();
    void mouseMoveEvent(QMouseEvent * e);
    void isLogin(bool flag);
    void setCombox(QString year,QString month,QString day);
    QString uid = "";  //保存登陆成功的用户名
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::search *ui;
    QString comboxMonth[12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};
    QString comboxDay[31] = {"01","02","03","04","05","06","07","08",
                             "09","10","11","12","13","14","15","16",
                             "17","18","19","20","21","22","23","24",
                             "25","26","27","28","29","30","31",};
};

#endif // SEARCH_H
