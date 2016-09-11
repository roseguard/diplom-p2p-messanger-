#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <string>э

#include <QCryptographicHash>

class registration : public QWidget
{
    Q_OBJECT

    QVBoxLayout* main_lay;

    QHBoxLayout *serv_lay;
    QLabel *serv_lab;
    QLineEdit *serv_edit;

    QHBoxLayout *servp_lay;
    QLabel *servp_lab;
    QLineEdit *servp_edit;


    QHBoxLayout* name_lay;
    QLabel *name_lab;
    QLineEdit *name_edit;

    QHBoxLayout* pass_lay;
    QLabel *pass_lab;
    QLineEdit *pass_edit;

    QHBoxLayout* port_lay;
    QLabel *port_lab;
    QLineEdit *port_edit;

    QHBoxLayout* ip_lay;
    QLabel *ip_lab;
    QLineEdit *ip_edit;

    QPushButton *regbutt;

    QTcpSocket *regsock;

    QString close_key;
    QString open_key;

    QMessageBox *mess;

public:
    void key_gen();

public slots:
    void reg_request();
    void get_ans();

public:
    registration(QWidget *parent = 0);
    ~registration();
};

#endif // REGISTRATION_H
