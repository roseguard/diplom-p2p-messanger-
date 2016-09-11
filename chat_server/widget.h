#ifndef WIDGET_H
#define WIDGET_H

//(name VARCHAR, password VARCHAR, port INT, cript VARCHAR, ip VARCHAR)
#define name_num 0
#define password_num 1
#define port_num 2
#define cript_num 3
#define ip_num 4

#include <QWidget>

#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QTcpServer>
#include <QTcpSocket>

#include <QVector>

class Widget : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout *main_lay;
    QTextBrowser *log;
    QPushButton *exit;

    QVector<QTcpSocket*> active_users;
    QTcpServer *rserver;

    QSqlDatabase *db;
    QSqlQuery *query;

public slots:
    void new_active_user();
    void new_message();

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_Hz
