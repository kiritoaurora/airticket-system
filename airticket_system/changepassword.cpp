#include "changepassword.h"
#include "ui_changepassword.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

changePassword::changePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePassword)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码界面");
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
}

changePassword::~changePassword()
{
    delete ui;
}

void changePassword::on_pushButton_2_clicked()  //取消按钮
{
    this->close();
}

void changePassword::on_pushButton_clicked()  //确认按钮
{
    QString sql =  QString("select password from usertable where username = '%1'").arg(uid);
    QSqlQuery query;
    query.exec(sql);
    query.next();
    QString pwd = query.value(0).toString();
    if(ui->lineEdit->text() != pwd)
    {
        QMessageBox::information(this,"修改信息","密码错误！");
    }
    else if(ui->lineEdit_2->text().length()<6)
    {
        QMessageBox::information(this,"修改信息","密码长度不能小于6！");
    }
    else if(ui->lineEdit_2->text() != ui->lineEdit_3->text())
    {
        QMessageBox::information(this,"修改信息","密码和确认密码不一致！");
    }
    else
    {
        QMessageBox::information(this,"修改信息","修改成功！");
        pwd = ui->lineEdit_2->text();
        sql = QString("update usertable set password = '%1' where username = '%2'")
                .arg(pwd).arg(uid);
        query.exec(sql);
        this->close();
    }
}
