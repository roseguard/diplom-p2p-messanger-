#include "adding_friend.h"

#include <QThread>
#include <QSqlError>

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
    database->setPassword("roseguard");
    query = new QSqlQuery;

    main_lay = new QVBoxLayout(this);
    radio_butt = new QHBoxLayout(this);
    by_name = new QRadioButton("За іменем", this);
    by_local = new QRadioButton("За даними", this);
    radio_butt->addWidget(by_name);
    radio_butt->addWidget(by_local);
    main_lay->addLayout(radio_butt);
    serv_lay = new QHBoxLayout(this);
    serv_lab = new QLabel(this);
    serv_lab->setText("Адрес сервера : ");
    serv_edit = new QLineEdit(this);
    serv_edit->setText("0.0.0.0");
    serv_lay->addWidget(serv_lab);
    serv_lay->addWidget(serv_edit);
    main_lay->addLayout(serv_lay);
    servp_lay = new QHBoxLayout(this);
    servp_lab = new QLabel(this);
    servp_lab->setText("Порт сервера : ");
    servp_edit = new QLineEdit(this);
    servp_edit->setText("21034");
    servp_lay->addWidget(servp_lab);
    servp_lay->addWidget(servp_edit);
    main_lay->addLayout(servp_lay);
    by_name_lay = new QHBoxLayout(this);
    name_lab = new QLabel("логін : ", this);
    name_edit = new QLineEdit(this);
    by_name_lay->addWidget(name_lab);
    by_name_lay->addWidget(name_edit);
    main_lay->addLayout(by_name_lay);

    by_local_lay = new QVBoxLayout(this);
    local_name_lay = new QHBoxLayout(this);
    local_name_lab = new QLabel("логін : ", this);
    local_name_edit = new QLineEdit(this);
    local_name_lay->addWidget(local_name_lab);
    local_name_lay->addWidget(local_name_edit);
    by_local_lay->addLayout(local_name_lay);
    local_ip_lay = new QHBoxLayout(this);
    local_ip_lab = new QLabel("ІР : ", this);
    local_ip_edit = new QLineEdit(this);
    local_ip_lay->addWidget(local_ip_lab);
    local_ip_lay->addWidget(local_ip_edit);
    by_local_lay->addLayout(local_ip_lay);
    local_port_lay = new QHBoxLayout(this);
    local_port_lab = new QLabel("порт : ", this);
    local_port_edit = new QLineEdit(this);
    local_port_lay->addWidget(local_port_lab);
    local_port_lay->addWidget(local_port_edit);
    by_local_lay->addLayout(local_port_lay);
    local_crypt_lay = new QHBoxLayout(this);
    local_crypt_lab = new QLabel("ключ : ", this);
    local_crypt_edit = new QLineEdit(this);
    local_crypt_lay->addWidget(local_crypt_lab);
    local_crypt_lay->addWidget(local_crypt_edit);
    by_local_lay->addLayout(local_crypt_lay);
    main_lay->addLayout(by_local_lay);

    butts = new QHBoxLayout(this);
        search = new QPushButton(this);
        search->setText("Додати");
        cancel = new QPushButton(this);
        cancel->setText("Відмінити");
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

    update = new QPushButton("Оновити інформацію про друзів",this);
    main_lay->addWidget(update);
    by_name->setChecked(true);
    sender = new QTcpSocket(this);
    connect(by_name, SIGNAL(clicked(bool)), this, SLOT(change_type()));
    connect(by_local, SIGNAL(clicked(bool)), this, SLOT(change_type()));
    connect(cancel, SIGNAL(pressed()), this, SLOT(close()));
    connect(search, SIGNAL(pressed()), this, SLOT(searching()));
    connect(update, SIGNAL(pressed()), this, SLOT(updating_start()));
}

adding_friend::~adding_friend(){}

void adding_friend::change_type(){
    if(by_name->isChecked()){
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
    else{
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

void adding_friend::updating_start(){
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
    if(for_mem == "ERROR"){
        qDebug() << rname << "no user";
        sender->disconnectFromHost();
        disconnect(sender, SIGNAL(readyRead()), this, SLOT(get_data()));
        return;
    }
    qDebug() << "rname" << rname;
    switch(send_num){
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
        if(from_parent->count()-1 > name_num){
            qDebug() << name_num;
            name_num++;
            emit update->pressed();
        }
        else{
            name_num = 0;
            break;
        }
        break;
    default:
        break;
    }
}

void adding_friend::searching(){
    query = new QSqlQuery;

    if(by_local->isChecked()){
        if(local_name_edit->text().length() < 6){
            mess = new QMessageBox("Error",
                                   "Мінімальний розмір імені : 6",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        if(local_ip_edit->text().isEmpty()){
            mess = new QMessageBox("Error",
                                   "Введіть адресу",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        QString tempstr = local_port_edit->text();
        if(tempstr.isEmpty()){
            mess = new QMessageBox("Error",
                                   "Введіть порт",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        for(int i = 0; i < tempstr.length(); i++){
            if(!tempstr.at(i).isNumber()){
                mess = new QMessageBox("Error",
                                       "Порт не коректний",
                                       QMessageBox::Information,
                                       0,
                                       0,
                                       QMessageBox::Ok | QMessageBox::Escape);
                mess->show();
                return;
            }
        }
        if(database->tables().isEmpty()){
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
                               "Друга додано",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
        return;
    }
    if(by_name->isChecked()){
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

void adding_friend::getting(){
    qDebug() << "Work";
    QString for_mem;
    QTextStream rstream(sender);
    rstream >> for_mem;
    if(for_mem == "ERROR"){
        mess = new QMessageBox("Error",
                               "Користувача не знайдено",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
        return;
    }
    switch(send_num){
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

void adding_friend::flush_to_db(bool update){
    qDebug() << "flush db";
    if(database->tables().isEmpty()){
        query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                   " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
    }

    if(update && !rname.isEmpty() && !rip.isEmpty() && !rport.isEmpty() && !rcrypt.isEmpty()){
        qDebug() << "removing rname" << rname;
        QString for_request = "DELETE FROM FRIENDS where name == '";
        for_request.append(rname);
        for_request.append("';");
        query = new QSqlQuery;
        query->clear();
        query->exec(for_request);
        qDebug() << query->lastError().text();
        debug_db();
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
        query->clear();
        query->exec(for_request);
        query->clear();
    }

    else if(!rname.isEmpty() && !rip.isEmpty() && !rport.isEmpty() && !rcrypt.isEmpty()){
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

        if(!update){
            mess = new QMessageBox("Succes",
                                   "Друга додано",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
    }
    debug_db();
}

void adding_friend::debug_db(){
    qDebug() << "=================================================";
    if(database->tables().isEmpty()){
        query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                   " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
    }

    query->exec("SELECT * FROM FRIENDS;");
    while(query->next()){
        qDebug() << "-------------------------------------";
        qDebug() << "name" << query->value(0).toString();
        qDebug() << "ip" << query->value(1).toString();
        qDebug() << "port" << query->value(2).toString();
        qDebug() << "crypt" << query->value(3).toString();
        qDebug() << "-------------------------------------";
    }
    qDebug() << "=================================================";
}
