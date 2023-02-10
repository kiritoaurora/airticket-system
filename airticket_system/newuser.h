#ifndef NEWUSER_H
#define NEWUSER_H

#include <QWidget>

namespace Ui {
class newUser;
}

class newUser : public QWidget
{
    Q_OBJECT

public:
    explicit newUser(QWidget *parent = nullptr);
    ~newUser();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::newUser *ui;
};

#endif // NEWUSER_H
