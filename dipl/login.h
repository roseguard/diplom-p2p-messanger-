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
#include <QRadioButton>
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
    QVBoxLayout *radio_butt;
    QRadioButton *set_serv;
    QRadioButton *get_serv;
    QRadioButton *nothing;
    QTcpSocket *for_ip;
    QString ip_req;
    QTcpSocket *for_port;
    QString port_req;
    QPushButton *accept;
    QPushButton *reg_new;
    QMessageBox *mess;
    Options *local_options;
    QString for_request[2];
    int req_num = 0;
    bool can_start = false;
    QTcpSocket *optsock;
    QTcpSocket *logsock;
public slots:
    void loging();
    void get_answer();
    void start_reg();
    void get_opt();
    void ischanged();
public:
    login(Options *_local_opt, QWidget *parent = 0);
    ~login();
};

#endif // LOGIN_H
