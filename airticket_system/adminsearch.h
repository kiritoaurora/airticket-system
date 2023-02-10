#ifndef ADMINSEARCH_H
#define ADMINSEARCH_H

#include <QDialog>

namespace Ui {
class adminSearch;
}

class adminSearch : public QDialog
{
    Q_OBJECT

public:
    explicit adminSearch(QWidget *parent = nullptr);
    ~adminSearch();
    QString oStation,eStation,date,airnum;
    void setCombox(QString year,QString month,QString day);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::adminSearch *ui;
    QString comboxMonth[12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};
    QString comboxDay[31] = {"01","02","03","04","05","06","07","08",
                             "09","10","11","12","13","14","15","16",
                             "17","18","19","20","21","22","23","24",
                             "25","26","27","28","29","30","31",};
};

#endif // ADMINSEARCH_H
