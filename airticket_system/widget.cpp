#include "widget.h"
#include "ui_widget.h"
#include "search.h"
#include "ui_search.h"
#include "newuser.h"
#include "ui_newuser.h"
#include "admininterface.h"
#include "ui_admininterface.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("登陆界面");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()   //登录按钮
{
    QString uid,pwd;
    uid = ui->lineEdit->text();
    pwd = ui->lineEdit_2->text();
    if(isAdmin == 0)
    {
        QString sql=QString("select * from usertable where username='%1' and password='%2'")
                .arg(uid).arg(pwd);
        QSqlQuery query;
        if(uid==""||pwd=="")
        {
           QMessageBox::information(this,"登录信息","用户名或密码不能为空！");
        }
        else if(query.exec(sql)==true&&query.next()==true)
        {
            search *search_widget = new search;
            search_widget->isLogin(true);
            search_widget->uid = uid;
            this->close();
            search_widget->show();
        }
        else
        {
            QMessageBox::information(this,"登录信息","密码或用户名错误！");
            ui->lineEdit_2->clear();
        }
    }
    else if(isAdmin == 1)
    {
        if(uid==""||pwd=="")
        {
           QMessageBox::information(this,"登录信息","用户名或密码不能为空！");
        }
        else if(uid == adminName && pwd == adminPwd)
        {
            AdminInterface *admin_widget = new AdminInterface;
            this->close();
            admin_widget->show();
        }
        else
        {
            QMessageBox::information(this,"登录信息","密码或用户名错误！");
            ui->lineEdit_2->clear();
        }
    }

}

void Widget::on_pushButton_4_clicked() //游客入口按钮
{
    search *search_widget = new search;
    search_widget->isLogin(false);
    this->close();
    search_widget->show();
}

void Widget::on_pushButton_2_clicked()  //注册账户按钮
{
    newUser *newUser_widget = new newUser;
    newUser_widget->show();
}

void Widget::on_pushButton_5_clicked()  //管理员按钮
{
    if(isAdmin == 0)
    {
        isAdmin = 1;
        ui->label_4->setText("        管理员登陆");
        ui->pushButton_5->setText("普通用户");
        ui->pushButton_2->hide();
        ui->label_3->setText(NULL);
        ui->pushButton_4->hide();
    }
    else if(isAdmin == 1)
    {
        isAdmin = 0;
        ui->label_4->setText("       民航订票系统");
        ui->pushButton_5->setText("管理员登录");
        ui->pushButton_2->show();
        ui->label_3->setText("    |");
        ui->pushButton_4->show();
    }
}
