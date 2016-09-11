#include "options.h"
#include "change_info.h"
options::options(Options *_to_save, bool server,QWidget *parent) :
    QWidget(parent)
{

    main_lay = new QVBoxLayout(this);

    _server = server;
    to_save = _to_save;

    ip_lay = new QHBoxLayout;
    for_ip = new QLabel(this);
    for_ip->setText("Here you can change your ip");
    ip = new QLineEdit(to_save->user_ip,this);
    ip_lay->addWidget(for_ip);
    ip_lay->addWidget(ip);

    port_lay = new QHBoxLayout;
    for_port = new QLabel(this);
    for_port->setText("Change this if default port already taked by other prog.");
    port = new QLineEdit(to_save->user_port,this);
    port_lay->addWidget(for_port);
    port_lay->addWidget(port);

    controll = new QHBoxLayout;
    accept = new QPushButton(this);
    accept->setText("Accept");
    cancel = new QPushButton(this);
    cancel->setText("Cancel");
    controll->addWidget(accept);
    controll->addWidget(cancel);

    main_lay->addLayout(ip_lay);
    main_lay->addLayout(port_lay);
    main_lay->addLayout(controll);

    if(_server)
    {
        account_change = new QHBoxLayout(this);
        change_server_pass = new QPushButton(this);
        change_server_pass->setText("CHANGE PASS");
        change_server_ip = new QPushButton(this);
        change_server_ip->setText("CHANGE IP");
        change_server_port = new QPushButton(this);
        change_server_port->setText("CHANGE PORT");
        change_server_crypt = new QPushButton(this);
        change_server_crypt->setText("CHANGE CRYPT");
        account_change->addWidget(change_server_pass);
        account_change->addWidget(change_server_ip);
        account_change->addWidget(change_server_port);
        account_change->addWidget(change_server_crypt);
        main_lay->addLayout(account_change);
    }



    this->setLayout(main_lay);

    connect(this->accept, SIGNAL(pressed()), this, SLOT(to_accept()));
    connect(this->cancel, SIGNAL(pressed()), this, SLOT(to_cancel()));
    if(_server){
        connect(change_server_pass, SIGNAL(pressed()), this, SLOT(ch_pass()));
        connect(change_server_ip, SIGNAL(pressed()), this, SLOT(ch_ip()));
        connect(change_server_port, SIGNAL(pressed()), this, SLOT(ch_port()));
        connect(change_server_crypt, SIGNAL(pressed()), this, SLOT(ch_crypt()));
    }
}

void options::to_accept()
{
    to_save->user_ip = ip->text();
    to_save->user_port = port->text();
    this->close();
}

void options::to_cancel()
{
    this->close();
}

void options::ch_pass()
{
    change_info *ch_info = new change_info("pass");
    ch_info->show();
}

void options::ch_ip()
{
    change_info *ch_info = new change_info("ip");
    ch_info->show();
}

void options::ch_port()
{
    change_info *ch_info = new change_info("port");
    ch_info->show();
}

void options::ch_crypt()
{
    change_info *ch_info = new change_info("crypt");
    ch_info->show();
}

options::~options()
{

}
