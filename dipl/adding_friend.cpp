#include "adding_friend.h"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "widget.h"

#define addr_num 50
#define char_size 64


adding_friend::adding_friend(QWidget *parent)
    : QWidget(parent)
{
    database = new QSqlDatabase;
    *database = QSqlDatabase::addDatabase("QSQLITE");
    database->setDatabaseName("./db_usr");
    database->open();
    query = new QSqlQuery;

    main_lay = new QVBoxLayout(this);

    radio_butt = new QHBoxLayout(this);
    by_name = new QRadioButton("By name", this);
    by_local = new QRadioButton("By data", this);
    radio_butt->addWidget(by_name);
    radio_butt->addWidget(by_local);
    main_lay->addLayout(radio_butt);

    serv_lay = new QHBoxLayout(this);
    serv_lab = new QLabel(this);
    serv_lab->setText("Server addr : ");
    serv_edit = new QLineEdit(this);
    serv_edit->setText("0.0.0.0");
    serv_lay->addWidget(serv_lab);
    serv_lay->addWidget(serv_edit);
    main_lay->addLayout(serv_lay);

    servp_lay = new QHBoxLayout(this);
    servp_lab = new QLabel(this);
    servp_lab->setText("Server port : ");
    servp_edit = new QLineEdit(this);
    servp_edit->setText("21034");
    servp_lay->addWidget(servp_lab);
    servp_lay->addWidget(servp_edit);
    main_lay->addLayout(servp_lay);

    by_name_lay = new QHBoxLayout(this);
    name_lab = new QLabel("name : ", this);
    name_edit = new QLineEdit(this);
    by_name_lay->addWidget(name_lab);
    by_name_lay->addWidget(name_edit);
    main_lay->addLayout(by_name_lay);


    by_local_lay = new QVBoxLayout(this);

    local_name_lay = new QHBoxLayout(this);
    local_name_lab = new QLabel("name : ", this);
    local_name_edit = new QLineEdit(this);
    local_name_lay->addWidget(local_name_lab);
    local_name_lay->addWidget(local_name_edit);
    by_local_lay->addLayout(local_name_lay);

    local_ip_lay = new QHBoxLayout(this);
    local_ip_lab = new QLabel("ip : ", this);
    local_ip_edit = new QLineEdit(this);
    local_ip_lay->addWidget(local_ip_lab);
    local_ip_lay->addWidget(local_ip_edit);
    by_local_lay->addLayout(local_ip_lay);

    local_port_lay = new QHBoxLayout(this);
    local_port_lab = new QLabel("port : ", this);
    local_port_edit = new QLineEdit(this);
    local_port_lay->addWidget(local_port_lab);
    local_port_lay->addWidget(local_port_edit);
    by_local_lay->addLayout(local_port_lay);

    local_crypt_lay = new QHBoxLayout(this);
    local_crypt_lab = new QLabel("crypt : ", this);
    local_crypt_edit = new QLineEdit(this);
    local_crypt_lay->addWidget(local_crypt_lab);
    local_crypt_lay->addWidget(local_crypt_edit);
    by_local_lay->addLayout(local_crypt_lay);


    main_lay->addLayout(by_local_lay);

    QHBoxLayout *butts;
    QPushButton *search;
    QPushButton *cancel;


    butts = new QHBoxLayout(this);
        search = new QPushButton(this);
        search->setText("search");
        cancel = new QPushButton(this);
        cancel->setText("cancel");
        butts->addWidget(search);
        butts->addWidget(cancel);

    main_lay->addLayout(butts);

    local_name_lab->hide();
    local_name_edit->hide();

    local_ip_lab->hide();
    local_ip_edit->hide();

    local_port_lab->hide();
    local_port_edit->hide();

    local_crypt_lab->hide();
    local_crypt_edit->hide();

    name_lab->show();
    name_edit->show();

    update = new QPushButton("update frinds list",this);
    main_lay->addWidget(update);

    by_name->setChecked(true);

    sender = new QTcpSocket(this);

    connect(by_name, SIGNAL(clicked(bool)), this, SLOT(change_type()));
    connect(by_local, SIGNAL(clicked(bool)), this, SLOT(change_type()));
    connect(cancel, SIGNAL(pressed()), this, SLOT(close()));
    connect(search, SIGNAL(pressed()), this, SLOT(searching()));
    connect(update, SIGNAL(pressed()), this, SLOT(updating_start()));
}

adding_friend::~adding_friend()
{

}

void adding_friend::change_type()
{
    if(by_name->isChecked())
    {
        local_name_lab->hide();
        local_name_edit->hide();

        local_ip_lab->hide();
        local_ip_edit->hide();

        local_port_lab->hide();
        local_port_edit->hide();

        local_crypt_lab->hide();
        local_crypt_edit->hide();

        servp_lab->show();
        servp_edit->show();
        serv_lab->show();
        serv_edit->show();
        name_lab->show();
        name_edit->show();
    }
    else
    {
        local_name_lab->show();
        local_name_edit->show();

        local_ip_lab->show();
        local_ip_edit->show();

        local_port_lab->show();
        local_port_edit->show();

        local_crypt_lab->show();
        local_crypt_edit->show();

        servp_lab->hide();
        servp_edit->hide();
        serv_lab->hide();
        serv_edit->hide();
        name_lab->hide();
        name_edit->hide();
    }
}

void adding_friend::updating_start()
{
    rname = from_parent->itemText(name_num);
    QTextStream rstream(sender);
    qDebug() << "creating request";
    for_send[0] = "GETIP ";
    for_send[1] = "GETPORT ";
    for_send[2] = "GETCRIPT ";
    for_send[0].append(rname);
    for_send[1].append(rname);
    for_send[2].append(rname);
    for_send[0].append(" end  ");
    for_send[1].append(" end  ");
    for_send[2].append(" end  ");
    send_num = 0;
    qDebug() << "try connect";
    connect(sender, SIGNAL(readyRead()), this, SLOT(get_data()));
    qDebug() << "SLOT connected";
    sender->disconnectFromHost();
    qDebug() << "disconnect";
    qDebug() << serv_edit->text() << servp_edit->text();
    sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
    qDebug() << "connect";
    rstream << for_send[0];
    qDebug() << "connect";
}

void adding_friend::get_data()
{
    qDebug() << "Work";
    QString for_mem;
    QTextStream rstream(sender);
    rstream >> for_mem;
    if(for_mem == "ERROR")
    {
        qDebug() << rname << "no user";
        sender->disconnectFromHost();
        disconnect(sender, SIGNAL(readyRead()), this, SLOT(get_data()));
    }
    qDebug() << "rname" << rname;
    switch(send_num)
    {
    case(0):
        rip = for_mem;
        qDebug() << "rip" << rip;
        send_num++;
        sender->disconnectFromHost();
        sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        rstream << for_send[1];
        break;
    case(1):
        rport = for_mem;
        qDebug() << "rport" << rport;
        send_num++;
        sender->disconnectFromHost();
        sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        rstream << for_send[2];
        break;
    case(2):
        rcrypt = for_mem;
        qDebug() << "rcrypt" << rcrypt;
        send_num++;
        sender->disconnectFromHost();
        disconnect(sender, SIGNAL(readyRead()), this, SLOT(get_data()));
        flush_to_db(true);
        if(from_parent->count() > name_num)
        {
            qDebug() << name_num;
            name_num++;
            emit update->pressed();
        }
        break;
    default:
        break;
    }
}

void adding_friend::searching()
{
    query = new QSqlQuery;

    if(by_local->isChecked())
    {
        if(local_name_edit->text().length() < 6)
        {
            mess = new QMessageBox("Error",
                                   "Minimum size of name : 6",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        if(local_ip_edit->text().isEmpty())
        {
            mess = new QMessageBox("Error",
                                   "enter your ip",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        QString tempstr = local_port_edit->text();
        if(tempstr.isEmpty())
        {
            mess = new QMessageBox("Error",
                                   "enter your port",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        for(int i = 0; i < tempstr.length(); i++)
        {
            if(!tempstr.at(i).isNumber())
            {
                mess = new QMessageBox("Error",
                                       "port is't valid",
                                       QMessageBox::Information,
                                       0,
                                       0,
                                       QMessageBox::Ok | QMessageBox::Escape);
                mess->show();
                return;
            }
        }

        if(database->tables().isEmpty())
        {
            query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                       " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
        };

        QString for_request = "INSERT INTO FRIENDS(name, ip, port, crypt) values('";
        for_request.append(local_name_edit->text());
        for_request.append("', '");
        for_request.append(local_ip_edit->text());
        for_request.append("', '");
        for_request.append(local_port_edit->text());
        for_request.append("', '");
        for_request.append(local_crypt_edit->text());
        for_request.append("');");

        qDebug() << for_request;
        query->exec(for_request);
        query->clear();

        mess = new QMessageBox("Succes",
                               "Friend added",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
        return;
    }
    if(by_name->isChecked())
    {
        rname = name_edit->text();
        QTextStream rstream(sender);
        qDebug() << "creating request";
        for_send[0] = "GETIP ";
        for_send[1] = "GETPORT ";
        for_send[2] = "GETCRIPT ";
        for_send[0].append(name_edit->text());
        for_send[1].append(name_edit->text());
        for_send[2].append(name_edit->text());
        for_send[0].append(" end  ");
        for_send[1].append(" end  ");
        for_send[2].append(" end  ");
        send_num = 0;
        qDebug() << "try connect";
        connect(sender, SIGNAL(readyRead()), this, SLOT(getting()));
        sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        rstream << for_send[0];
        qDebug() << "connect";
    }
}

void adding_friend::getting()
{
    qDebug() << "Work";
    QString for_mem;
    QTextStream rstream(sender);
    rstream >> for_mem;
    if(for_mem == "ERROR")
    {
        mess = new QMessageBox("Error",
                               "Name not found",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
        return;
    }
    switch(send_num)
    {
    case(0):
        rip = for_mem;
        send_num++;
        sender->disconnectFromHost();
        qDebug() << "disconnect";
        sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        qDebug() << "connect";
        rstream << for_send[1];
        break;
    case(1):
        rport = for_mem;
        send_num++;
        sender->disconnectFromHost();
        qDebug() << "disconnect";
        sender->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        qDebug() << "connect";
        rstream << for_send[2];
        break;
    case(2):
        rcrypt = for_mem;
        send_num++;
        sender->disconnectFromHost();
        qDebug() << "disconnect";
        disconnect(sender, SIGNAL(readyRead()), this, SLOT(getting()));
        flush_to_db(false);
        break;
    default:
        break;
    }
}

void adding_friend::flush_to_db(bool update)
{
    qDebug() << "flush db";
    if(database->tables().isEmpty())
    {
        query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                   " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
    }

    if(update && !rname.isEmpty() && !rip.isEmpty() && !rport.isEmpty() && !rcrypt.isEmpty())
    {
        qDebug() << "removing rname" << rname;
        QString for_request = "DELETE FROM FRIEND where name == '";
        for_request.append(rname);
        for_request.append("';");
        query->exec(for_request);
        for_request = "INSERT INTO FRIENDS(name, ip, port, crypt) values('";
        for_request.append(rname);
        for_request.append("', '");
        for_request.append(rip);
        for_request.append("', '");
        for_request.append(rport);
        for_request.append("', '");
        for_request.append(rcrypt);
        for_request.append("');");

        qDebug() << for_request;
        query->exec(for_request);
        query->clear();
    }

    if(!rname.isEmpty() && !rip.isEmpty() && !rport.isEmpty() && !rcrypt.isEmpty())
    {
        qDebug() << "adding name" << rname;
        QString for_request = "INSERT INTO FRIENDS(name, ip, port, crypt) values('";
        for_request.append(rname);
        for_request.append("', '");
        for_request.append(rip);
        for_request.append("', '");
        for_request.append(rport);
        for_request.append("', '");
        for_request.append(rcrypt);
        for_request.append("');");

        qDebug() << for_request;
        query->exec(for_request);
        query->clear();

        if(!update)
        {
            mess = new QMessageBox("Succes",
                                   "Friend added",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
    }
}
