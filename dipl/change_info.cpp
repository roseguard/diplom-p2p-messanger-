#include "change_info.h"
#include <QListView>
#include <QModelIndex>

change_info::change_info(QString _for_change, QWidget *parent)
    : QWidget(parent)
{
    for_change = _for_change;
    main_lay = new QVBoxLayout(this);

    serv_lay = new QHBoxLayout(this);
    serv_lab = new QLabel(this);
    serv_lab->setText("Server addr : ");
    serv_edit = new QLineEdit(this);
    serv_edit->setText("0.0.0.0");
    serv_lay->addWidget(serv_lab);
    serv_lay->addWidget(serv_edit);

    servp_lay = new QHBoxLayout(this);
    servp_lab = new QLabel(this);
    servp_lab->setText("Server port : ");
    servp_edit = new QLineEdit(this);
    servp_edit->setText("21034");
    servp_lay->addWidget(servp_lab);
    servp_lay->addWidget(servp_edit);

    old_name_lay = new QHBoxLayout(this);
    old_name_lab = new QLabel(this);
    old_name_lab->setText("old name");
    old_name_edit = new QLineEdit(this);
    old_name_lay->addWidget(old_name_lab);
    old_name_lay->addWidget(old_name_edit);

    old_pass_lay = new QHBoxLayout(this);
    old_pass_lab = new QLabel(this);
    old_pass_lab->setText("old password");
    old_pass_edit = new QLineEdit(this);
    old_pass_lay->addWidget(old_pass_lab);
    old_pass_lay->addWidget(old_pass_edit);

    main_lay->addLayout(serv_lay);
    main_lay->addLayout(servp_lay);
    main_lay->addLayout(old_name_lay);
    main_lay->addLayout(old_pass_lay);


    if(_for_change == "name"){
        name_lay = new QHBoxLayout(this);
        name_lab = new QLabel(this);
        name_lab->setText("new name");
        name_edit = new QLineEdit(this);
        name_lay->addWidget(name_lab);
        name_lay->addWidget(name_edit);
        main_lay->addLayout(name_lay);
    }
    else if(_for_change == "pass"){
        pass_lay = new QHBoxLayout(this);
        pass_lab = new QLabel(this);
        pass_lab->setText("new password");
        pass_edit = new QLineEdit(this);
        pass_lay->addWidget(pass_lab);
        pass_lay->addWidget(pass_edit);
        main_lay->addLayout(pass_lay);
    }
    else if(_for_change == "ip"){
        ip_lay = new QHBoxLayout(this);
        ip_lab = new QLabel(this);
        ip_lab->setText("new ip");
        ip_edit = new QLineEdit(this);
        ip_lay->addWidget(ip_lab);
        ip_lay->addWidget(ip_edit);
        main_lay->addLayout(ip_lay);
    }
    else if(_for_change == "port"){
        port_lay = new QHBoxLayout(this);
        port_lab = new QLabel(this);
        port_lab->setText("new port");
        port_edit = new QLineEdit(this);
        port_lay->addWidget(port_lab);
        port_lay->addWidget(port_edit);
        main_lay->addLayout(port_lay);
    }


    chbutt = new QPushButton(this);
    chbutt->setText("set changing");
    main_lay->addWidget(chbutt);

    connect(chbutt, SIGNAL(pressed()), this, SLOT(sending_changing()));
}

change_info::~change_info()
{
}

void change_info::sending_changing()
{
    QLineEdit *changing_line;
    if(for_change == "name"){
        changing_line = name_edit;
        if(name_edit->text().length() < 6)
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
    }
    else if(for_change == "pass"){
        changing_line = pass_edit;
        if(pass_edit->text().length() < 6)
        {
            mess = new QMessageBox("Error",
                                   "Minimum size of password : 6",
                                   QMessageBox::Information,
                                   0,
                                   0,
                                   QMessageBox::Ok | QMessageBox::Escape);
            mess->show();
            return;
        }
    }
    else if(for_change == "ip"){
        QString tempstr = ip_edit->text();
        changing_line = ip_edit;
        if(ip_edit->text().isEmpty())
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
    }
    else if(for_change == "port"){
        changing_line = port_edit;
        QString tempstr = port_edit->text();
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
    }
    else if(for_change == "crypt"){
        gen_public_key();
    }

    regsock = new QTcpSocket(this);
    connect(regsock, SIGNAL(readyRead()), this, SLOT(getting_answer()));
    regsock->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
    QTextStream b(regsock);
    QString for_request = "SET";
    for_request.append(for_change);
    for_request.append(" ");
    for_request.append(old_name_edit->text());
    for_request.append(" ");
    QCryptographicHash qcript(QCryptographicHash::Md5);
    qcript.addData(old_pass_edit->text().toLatin1());
    QString for_pass_hash = "";
    for(int i = 0; i < qcript.result().length(); i++)
        for_pass_hash.append(QString::number(qcript.result().at(i)));
    qDebug() << for_pass_hash;
    for_request.append(for_pass_hash);
    for_request.append(" ");

    if(for_change == "name")      for_request.append(name_edit->text());
    else if(for_change == "pass"){
        QCryptographicHash qcrypt(QCryptographicHash::Md5);
        qcrypt.addData(pass_edit->text().toLatin1());
        QString for_pass_hash = "";
        for(int i = 0; i < qcrypt.result().length(); i++)
            for_pass_hash.append(QString::number(qcrypt.result().at(i)));
        qDebug() << for_pass_hash;
        for_request.append(for_pass_hash);
    }
    else if(for_change == "ip")   for_request.append(ip_edit->text());
    else if(for_change == "port") for_request.append(port_edit->text());
    else if(for_change == "crypt")for_request.append(open_key);
    for_request.append(" end ");
    qDebug() << for_request;
    b << for_request;
}






void change_info::getting_answer()
{
    QTextStream b(regsock);
    QString for_ans;
    b >> for_ans;
    if(for_ans!="SUCCES")
    {
        mess = new QMessageBox("Error",
                               for_ans,
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
    }
    qDebug() << for_ans;
}

void change_info::gen_public_key()
{
    open_key = "tempkey";
}
