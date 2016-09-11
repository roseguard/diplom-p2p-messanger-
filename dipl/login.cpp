#include "login.h"
#include "registration.h"
#include "chat_menu.h"
#include "options.h"

login::login(Options* _local_opt ,QWidget *parent)
    : QWidget(parent)
{
    local_options = _local_opt;
    main_lay = new QVBoxLayout(this);
    serv_lay = new QHBoxLayout(this);
    serv_lab = new QLabel(this);
    serv_lab->setText("Адрес сервера : ");
    serv_edit = new QLineEdit(this);
    serv_edit->setText("0.0.0.0");
    serv_lay->addWidget(serv_lab);
    serv_lay->addWidget(serv_edit);
    servp_lay = new QHBoxLayout(this);
    servp_lab = new QLabel(this);
    servp_lab->setText("Порт сервера : ");
    servp_edit = new QLineEdit(this);
    servp_edit->setText("21034");
    servp_lay->addWidget(servp_lab);
    servp_lay->addWidget(servp_edit);
    login_lay = new QHBoxLayout(this);
    login_lab = new QLabel(this);
    login_lab->setText("Логін : ");
    login_edit = new QLineEdit(this);
    login_lay->addWidget(login_lab);
    login_lay->addWidget(login_edit);
    pass_lay = new QHBoxLayout(this);
    pass_lab = new QLabel(this);
    pass_lab->setText("Пароль : ");
    pass_edit = new QLineEdit(this);
    pass_lay->addWidget(pass_lab);
    pass_lay->addWidget(pass_edit);
    radio_butt = new QVBoxLayout(this);
    set_serv = new QRadioButton("Відправити налаштування на сервер", this);
    get_serv = new QRadioButton("Отримати налаштування від сервера", this);
    nothing = new QRadioButton("Ніяких додаткових опцій з сервром", this);
    nothing->setChecked(true);
    radio_butt->addWidget(set_serv);
    radio_butt->addWidget(get_serv);
    radio_butt->addWidget(nothing);
    accept = new QPushButton("Логін",this);
    reg_new = new QPushButton("Реєстрація",this);
    main_lay->addLayout(serv_lay);
    main_lay->addLayout(servp_lay);
    main_lay->addLayout(login_lay);
    main_lay->addLayout(pass_lay);
    main_lay->addLayout(radio_butt);
    main_lay->addWidget(accept);
    main_lay->addWidget(reg_new);
    connect(accept, SIGNAL(pressed()), this, SLOT(loging()));
    connect(reg_new, SIGNAL(pressed()), this, SLOT(start_reg()));
}

login::~login(){
}

void login::loging(){
    logsock = new QTcpSocket(this);
    connect(logsock, SIGNAL(readyRead()), this, SLOT(get_answer()));
    logsock->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
    QTextStream b(logsock);
    QString for_request = "LOGUSER ";
    for_request.append(login_edit->text());
    for_request.append(" ");
    QCryptographicHash qcrypt(QCryptographicHash::Md5);
    qcrypt.addData(pass_edit->text().toLatin1());
    QString for_pass_hash = "";
    for(int i = 0; i < qcrypt.result().length(); i++)
        for_pass_hash.append(QString::number(qcrypt.result().at(i)));
    qDebug() << for_pass_hash;
    for_request.append(for_pass_hash);
    for_request.append(" end ");
    b << for_request;
}

void login::get_answer(){
    QTextStream b(logsock);
    QString for_ans;
    b >> for_ans;
    if(for_ans!="SUCCES"){
        mess = new QMessageBox("Error",
                               for_ans,
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
    }
    else{
        if(nothing->isChecked()){
            chat_menu *chat = new chat_menu(local_options, 1);
            chat->show();
            this->close();
        }
        if(set_serv->isChecked()){
            for_ip = new QTcpSocket(this);
            for_port = new QTcpSocket(this);
            connect(for_ip, SIGNAL(readyRead()), this, SLOT(ischanged()));
            connect(for_port, SIGNAL(readyRead()), this, SLOT(ischanged()));
            for_ip->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
            for_port->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
            QTextStream istream(for_ip);
            QTextStream pstream(for_port);
            ip_req = "SETip " + login_edit->text() + " ";
            QCryptographicHash qcript(QCryptographicHash::Md5);
            qcript.addData(pass_edit->text().toLatin1());
            QString for_pass_hash = "";
            for(int i = 0; i < qcript.result().length(); i++)
                for_pass_hash.append(QString::number(qcript.result().at(i)));
            ip_req.append(for_pass_hash + " " + local_options->user_ip + " end ");
            port_req = "SETport " + login_edit->text() + " " + for_pass_hash + " " + local_options->user_port + " end ";
            can_start = false;
            istream << ip_req;
            pstream << port_req;
        }
        if(get_serv->isChecked()){
            qDebug() << "I alive 1111111111111111111111111";
            logsock->close();
            disconnect(logsock, SIGNAL(readyRead()), this, SLOT(get_answer()));
            connect(logsock, SIGNAL(readyRead()), this, SLOT(get_opt()));
            for_request[0] = "GETIP ";
            for_request[0].append(login_edit->text());
            for_request[0].append(" end ");
            for_request[1] = "GETPORT ";
            for_request[1].append(login_edit->text());
            for_request[1].append(" end ");
            qDebug() << "req formed";
            logsock->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
            QTextStream rstream(logsock);
            rstream << for_request[0];
        }
    }
    qDebug() << for_ans;
}

void login::ischanged(){
    QString for_mem;
    QTextStream istream(for_ip);
    istream >> for_mem;
    if(!for_mem.isEmpty()){
        if(for_mem != "SUCCES"){
            mess = new QMessageBox("Error",
                                   "Сервер повідомив про помилку",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            disconnect(for_ip, SIGNAL(readyRead()), this, SLOT(ischanged()));
            disconnect(for_port, SIGNAL(readyRead()), this, SLOT(ischanged()));
            for_ip->disconnectFromHost();
            for_ip->close();
            for_port->disconnectFromHost();
            for_port->close();
            return;
        }
        else{
            if(can_start){
                chat_menu *chat = new chat_menu(local_options, 1);
                chat->show();
                this->close();
            }
            else{
                can_start = true;
            }
        }
    }
    else{
        istream.setDevice(for_port);
        istream >> for_mem;
        if(for_mem.isEmpty()){
            mess = new QMessageBox("Error",
                                   "Повідомлення не знайдено",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
        else{
            if(for_mem != "SUCCES"){
                mess = new QMessageBox("Error",
                                       "Сервер повідомляє про помилку",
                                       QMessageBox::Information,
                                       0,
                                       0,
                                       QMessageBox::Ok | QMessageBox::Escape);
                mess->show();
                disconnect(for_ip, SIGNAL(readyRead()), this, SLOT(ischanged()));
                disconnect(for_port, SIGNAL(readyRead()), this, SLOT(ischanged()));
                for_ip->disconnectFromHost();
                for_ip->close();
                for_port->disconnectFromHost();
                for_port->close();
                return;
            }
            else{
                if(can_start){
                    chat_menu *chat = new chat_menu(local_options, 1);
                    chat->show();
                    this->close();
                }
                else{
                    can_start = true;
                }
            }
        }
    }
}

void login::get_opt(){
    qDebug() << "I'm 22222222222222222222222222";
    QString for_mem;
    QTextStream rstream(logsock);
    rstream >> for_mem;
    if(for_mem == "ERROR"){
        mess = new QMessageBox("Error",
                               "Сервер повідомляє про помилку",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
    }
    switch(req_num){
    case(0):
        local_options->user_ip = for_mem;
        req_num++;
        logsock->close();
        logsock->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
        rstream << for_request[1];
        break;
    case(1):
        local_options->user_port = for_mem;
        req_num = 0;
        logsock->close();
        chat_menu *chat = new chat_menu(local_options, 1);
        chat->show();
        this->close();
        break;
    }
}

void login::start_reg(){
    registration *newreg = new registration;
    newreg->show();
}
