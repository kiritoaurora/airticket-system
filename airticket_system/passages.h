#ifndef PASSAGES_H
#define PASSAGES_H

#include <QDialog>

namespace Ui {
class Passages;
}

class Passages : public QDialog
{
    Q_OBJECT

public:
    explicit Passages(QWidget *parent = nullptr);
    ~Passages();
    QString id,name,pnum;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Passages *ui;
};

#endif // PASSAGES_H
