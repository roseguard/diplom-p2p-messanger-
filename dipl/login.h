#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QDebug>
#include <QCryptographicHash>

#include "options.h"

class login : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout *main_lay;

    QHBoxLayout *serv_lay;
    QLabel *serv_lab;
    QLineEdit *serv_edit;

    QHBoxLayout *servp_lay;
    QLabel *servp_lab;
    QLineEdit *servp_edit;

    QHBoxLayout *login_lay;
    QLabel *login_lab;
    QLineEdit *login_edit;

    QHBoxLayout *pass_lay;
    QLabel *pass_lab;
    QLineEdit *pass_edit;

    QPushButton *accept;
    QPushButton *reg_new;

    QMessageBox *mess;

    Options *local_options;

public:
    QTcpSocket *logsock;

public slots:
    void loging();
    void loging_cont();
    void get_answer();
    void start_reg();

public:
    login(Options *_local_opt, QWidget *parent = 0);
    ~login();
};

#endif // LOGIN_H
