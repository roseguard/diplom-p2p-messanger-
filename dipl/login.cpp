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

    login_lay = new QHBoxLayout(this);
    login_lab = new QLabel(this);
    login_lab->setText("Login : ");
    login_edit = new QLineEdit(this);
    login_lay->addWidget(login_lab);
    login_lay->addWidget(login_edit);

    pass_lay = new QHBoxLayout(this);
    pass_lab = new QLabel(this);
    pass_lab->setText("Password : ");
    pass_edit = new QLineEdit(this);
    pass_lay->addWidget(pass_lab);
    pass_lay->addWidget(pass_edit);

    accept = new QPushButton(this);
    accept->setText("LOGIN");

    reg_new = new QPushButton(this);
    reg_new->setText("REGISTER");

    main_lay->addLayout(serv_lay);
    main_lay->addLayout(servp_lay);
    main_lay->addLayout(login_lay);
    main_lay->addLayout(pass_lay);
    main_lay->addWidget(accept);
    main_lay->addWidget(reg_new);

    connect(accept, SIGNAL(pressed()), this, SLOT(loging()));
    connect(reg_new, SIGNAL(pressed()), this, SLOT(start_reg()));
}

login::~login()
{

}

void login::loging()
{
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

void login::loging_cont()
{

}

void login::get_answer()
{
    QTextStream b(logsock);
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
        chat_menu *chat = new chat_menu(local_options, 1);
        chat->show();
        this->close();
    }
    qDebug() << for_ans;
}

void login::start_reg()
{
    registration *newreg = new registration;
    newreg->show();
}
