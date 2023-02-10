#include "newuser.h"
#include "ui_newuser.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
newUser::newUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::newUser)
{
    ui->setupUi(this);
    this->setWindowTitle("注册界面");
}

newUser::~newUser()
{
    delete ui;
}

void newUser::on_pushButton_clicked()  //注册按钮
{
    QString uid,pwd,pwd2;
    uid = ui->lineEdit->text();
    pwd = ui->lineEdit_2->text();
    pwd2 = ui->lineEdit_3->text();
    QString sql = QString("select username from usertable where username = '%1'").arg(uid);
    QSqlQuery query;
    if(uid==""||pwd==""||pwd=="")
    {
       QMessageBox::information(this,"注册信息","用户名或密码不能为空！");
    }
    else if(pwd.length()<6)
    {
        QMessageBox::information(this,"注册信息","密码长度不能小于6！");
    }
    else if(pwd != pwd2)
    {
        QMessageBox::information(this,"注册信息","密码和确认密码不一致！");
    }
    else if(query.exec(sql)==true&&query.next()==true)
    {
        QMessageBox::information(this,"注册信息","用户名已存在！");
    }
    else
    {
        QMessageBox::information(this,"注册信息","注册成功！");
        sql = QString("insert into usertable(username,password) values('%1','%2')")
                .arg(uid).arg(pwd);
        query.exec(sql);
        this->close();
    }
}

void newUser::on_pushButton_2_clicked()
{
    this->close();
}
