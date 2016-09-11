// класс для зміни налаштувань

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

#include <QComboBox>
#include <QTextBlock>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include "widget.h"

class options : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout *main_lay;
    QHBoxLayout *ip_lay;
    QLabel *for_ip;
    QLineEdit *ip;
    QHBoxLayout *port_lay;
    QLabel *for_port;
    QLineEdit *port;
    QHBoxLayout *controll;
    QPushButton *accept;
    QPushButton *cancel;
    QHBoxLayout *account_change;
    QPushButton *change_server_pass;
    QPushButton *change_server_ip;
    QPushButton *change_server_port;
    QPushButton *change_server_crypt;
    Options *to_save;
    bool _server;
public slots:
    void to_accept();
    void to_cancel();
    void ch_pass();
    void ch_ip();
    void ch_port();
    void ch_crypt();
public:
    explicit options(Options *_to_save,bool server = false,QWidget *parent = 0);
    ~options();
};
#endif // OPTIONS_H
