#ifndef UPDATEFLIGHTINFO_H
#define UPDATEFLIGHTINFO_H

#include <QDialog>

namespace Ui {
class updateFlightInfo;
}

class updateFlightInfo : public QDialog
{
    Q_OBJECT

public:
    explicit updateFlightInfo(QWidget *parent = nullptr);
    ~updateFlightInfo();
    QString info[9];
    void setInfo();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::updateFlightInfo *ui;
};

#endif // UPDATEFLIGHTINFO_H
