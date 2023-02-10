#include "widget.h"

#include <QApplication>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

bool OpenDataBase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    qDebug()<<"ODBC driver?"<<db.isValid();
    QString dsn = QString::fromLocal8Bit("QTDSN");
    db.setHostName("localhost");
    db.setDatabaseName(dsn);
    db.setUserName("sa");
    db.setPassword("s1400117627");
    if(!db.open())
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0,QObject::tr("Database error"),db.lastError().text());
        return false;
    }
    else
        qDebug()<<"Database open success!";
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    if(!OpenDataBase())
        return 1;
//    QString sql = ("select * from usertable");
//    QSqlQuery query;
//    query.exec(sql);
//    query.next();
//    qDebug()<<query.value(0).toString()<<query.value(1).toString();
    w.show();
    return a.exec();
}
