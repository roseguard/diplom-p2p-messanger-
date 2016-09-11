#include "registration.h"

registration::registration(QWidget *parent)
    : QWidget(parent)
{
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
    name_lay = new QHBoxLayout(this);
    name_lab = new QLabel(this);
    name_lab->setText("name");
    name_edit = new QLineEdit(this);
    name_lay->addWidget(name_lab);
    name_lay->addWidget(name_edit);

    pass_lay = new QHBoxLayout(this);
    pass_lab = new QLabel(this);
    pass_lab->setText("password");
    pass_edit = new QLineEdit(this);
    pass_lay->addWidget(pass_lab);
    pass_lay->addWidget(pass_edit);

    ip_lay = new QHBoxLayout(this);
    ip_lab = new QLabel(this);
    ip_lab->setText("your ip");
    ip_edit = new QLineEdit(this);
    ip_lay->addWidget(ip_lab);
    ip_lay->addWidget(ip_edit);

    port_lay = new QHBoxLayout(this);
    port_lab = new QLabel(this);
    port_lab->setText("your port");
    port_edit = new QLineEdit(this);
    port_lay->addWidget(port_lab);
    port_lay->addWidget(port_edit);

    regbutt = new QPushButton(this);
    regbutt->setText("registr");

    main_lay->addLayout(serv_lay);
    main_lay->addLayout(servp_lay);
    main_lay->addLayout(name_lay);
    main_lay->addLayout(pass_lay);
    main_lay->addLayout(ip_lay);
    main_lay->addLayout(port_lay);
    main_lay->addWidget(regbutt);

    connect(regbutt, SIGNAL(pressed()), this, SLOT(reg_request()));
}

registration::~registration()
{

}

void registration::reg_request()
{
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
    regsock = new QTcpSocket(this);
    connect(regsock, SIGNAL(readyRead()), this, SLOT(get_ans()));
    regsock->connectToHost(QHostAddress(serv_edit->text()), servp_edit->text().toInt());
    QTextStream b(regsock);
    QString for_request = "REGUSER ";
    for_request.append(name_edit->text());
    for_request.append(" ");
    QCryptographicHash qcript(QCryptographicHash::Md5);
    qcript.addData(pass_edit->text().toLatin1());
    QString for_pass_hash = "";
    for(int i = 0; i < qcript.result().length(); i++)
        for_pass_hash.append(QString::number(qcript.result().at(i)));
    qDebug() << for_pass_hash;
    for_request.append(for_pass_hash);
    for_request.append(" ");
    for_request.append(port_edit->text());
    for_request.append(" ");
    key_gen();
    for_request.append(open_key);
    for_request.append(" ");
    for_request.append(ip_edit->text());
    for_request.append(" end ");
    b << for_request;
}

void registration::get_ans()
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
    else
    {
        mess = new QMessageBox("Succes",
                               "Registration succes",
                               QMessageBox::Information,
                               0,
                               0,
                               QMessageBox::Ok | QMessageBox::Escape);
        mess->show();
    }
    qDebug() << for_ans;
}

void registration::key_gen()
{
    open_key = "tempkey";
    close_key = "tempkey";
    /*
    QString kname = name_edit->text();
    QString kpass = pass_edit->text();
    while (kname.length() < kpass.length()) kname.append("a");
    while (kname.length() > kpass.length()) kpass.append("a");
    u_int8_t *tkey = new u_int8_t[kname.length()];
    QString key;
    for(int i = 0; i < kpass.length(); i++)
    {
        key.append(" ")
        qDebug() << tkey[i] = kpass.at(i).toLatin1() + kpass.at(i).toLatin1();
    }

    qDebug() << key;
    */
}
