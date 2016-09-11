#include "widget.h"
#include "options.h"
#include "chat_menu.h"
#include "login.h"
#include "QIcon"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("");
    start_with_login = new QPushButton(this);
    start_with_login->setText("Запустити з використанням сервера");
    start_without_login = new QPushButton(this);
    start_without_login->setText("Запустити без використання сервера");
    options_butt = new QPushButton(this);
    options_butt->setText("Налаштування");
    exit_butt = new QPushButton(this);
    exit_butt->setText("Вийти");
    lay = new QVBoxLayout(this);
    lay->addWidget(start_with_login);
    lay->addWidget(start_without_login);
    lay->addWidget(options_butt);
    lay->addWidget(exit_butt);
    this->setLayout(lay);
    User_options = new Options;
    User_options->user_ip = "127.0.0.1";
    User_options->user_port = "21035";
    connect(this->options_butt, SIGNAL(pressed()), this, SLOT(open_options()));
    connect(this->start_with_login, SIGNAL(pressed()), this, SLOT(start_login()));
    connect(this->start_without_login, SIGNAL(pressed()), this, SLOT(start_chat()));
    connect(this->exit_butt, SIGNAL(pressed()), this, SLOT(close()));
}
Widget::~Widget(){
}
void Widget::start_login(){
    login *newlog = new login(User_options);
    newlog->show();
    this->close();
}
void Widget::start_chat(){
    chat_menu *chat;
    chat = new chat_menu(User_options, false);
    this->close();
    chat->show();
}
void Widget::open_options(){
    options *A = new options(User_options,false);
    A->show();
}
