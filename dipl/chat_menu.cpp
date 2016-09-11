#include "chat_menu.h"

#include "options.h"

#include "adding_friend.h"
#include "removing_friend.h"

chat_menu::chat_menu(Options *_opts, bool server, QWidget *parent) :
    QWidget(parent)
{
    _server = server;

    Local_options = _opts;

    db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("./db_usr");
    db->open();
    query = new QSqlQuery;

    rserver = new QTcpServer(this);
    connect(rserver, SIGNAL(newConnection()), this, SLOT(new_connection()));
    qDebug() << Local_options->user_port;
    rserver->listen(QHostAddress(Local_options->user_ip), Local_options->user_port.toInt());

    if(db->tables().isEmpty())
    {
        query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                   " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
    }

    main_lay = new QHBoxLayout(this);

    message_lay = new QVBoxLayout;
    messages = new QTextBrowser;
    sending_message_lay = new QHBoxLayout;
    sending_message = new QLineEdit;
    send_message = new QPushButton("send", this);
    friends_list = new QComboBox;
    sending_message_lay->addWidget(sending_message);
    message_lay->addWidget(messages);
    message_lay->addLayout(sending_message_lay);

    friends_lay = new QVBoxLayout;
    add_friend = new QPushButton("add friends", this);
    remove_friend = new QPushButton("remove friend", this);
    change_options = new QPushButton("options", this);
    exit = new QPushButton("exit", this);
    friends_lay->addWidget(add_friend);
    friends_lay->addWidget(remove_friend);
    friends_lay->addWidget(change_options);
    friends_lay->addWidget(exit);
    friends_lay->addWidget(friends_list);
    friends_lay->addWidget(send_message);

    main_lay->addLayout(message_lay);
    main_lay->addLayout(friends_lay);

    this->setLayout(main_lay);

    connect(change_options, SIGNAL(pressed()), this, SLOT(changing_options()));
    connect(add_friend, SIGNAL(pressed()), this, SLOT(f_adding_friend()));
    connect(remove_friend, SIGNAL(pressed()), this, SLOT(f_removing_friend()));
    connect(send_message, SIGNAL(pressed()), this, SLOT(new_send_message()));

    connect(exit, SIGNAL(pressed()), this, SLOT(end_chat()));

    update_list();
    list_timer = new QTimer(this);
    list_timer->singleShot(5000, this, SLOT(update_list()));
}

void chat_menu::end_chat()
{
    this->close();
}

void chat_menu::new_connection()
{
    QTcpSocket *for_save = new QTcpSocket(this);
    for_save = rserver->nextPendingConnection();
    recv_socks.push_back(for_save);
    connect(for_save, SIGNAL(readyRead()), this, SLOT(new_recv_message()));
    qDebug() << "new one";
}

void chat_menu::changing_options()
{
    A = new options(Local_options ,_server);
    connect(A->accept, SIGNAL(pressed()), this, SLOT(restart_serv()));
    A->show();
}

void chat_menu::f_adding_friend()
{
    adding_friend *ad = new adding_friend;
    ad->from_parent = this->friends_list;
    ad->show();
}

void chat_menu::f_removing_friend()
{
    removing_friend *rm_fr = new removing_friend;
    rm_fr->show();
}

chat_menu::~chat_menu()
{

}

void chat_menu::update_list()
{
    combo_num = friends_list->currentIndex();
    friends_list->clear();
    query = new QSqlQuery;
    query->exec("SELECT * FROM FRIENDS;");
    while(query->next())
    {
        friends_list->addItem(query->value(0).toString());
        struct db_list templist;
        templist.name = query->value(0).toString();
        templist.ip = query->value(1).toString();
        templist.port = query->value(2).toString();
        templist.crypt = query->value(3).toString();
        templist.sock_num = -1;
        friends_vector.push_back(templist);
    }
    friends_list->setCurrentIndex(combo_num);
    list_timer->singleShot(5000, this, SLOT(update_list()));
}

void chat_menu::new_recv_message()
{
    qDebug() << "new mess";
    QString tempstr;
    QVector<QString>datas;
    QTextStream rstream;
    int i;
    for(i = 0; i < recv_socks.size(); i++)
    {
        rstream.setDevice(recv_socks.at(i));
        rstream >> tempstr;
        if(tempstr.isEmpty())
            continue;
        else
            break;
    }
    while(tempstr!="" && tempstr!="end")
    {
        datas.push_back(tempstr);
        rstream >> tempstr;
    }
    tempstr.clear();
    for(int l = 0; l < datas.size(); l++)
    {
        tempstr.append((datas.at(l)));
        tempstr.append(" ");
    }
    messages->append(tempstr);
}

void chat_menu::new_send_message()
{
    QTextStream rstream;
    int i;
    for(i = 0; i < friends_list->count(); i++)
        if(friends_list->itemText(friends_list->currentIndex()) == friends_vector.at(i).name)
            break;
    if(friends_vector.at(i).sock_num==-1)
    {
        QTcpSocket *new_send_sock = new QTcpSocket;
        qDebug() << friends_vector.at(i).port;
        new_send_sock->connectToHost(QHostAddress(friends_vector.at(i).ip), friends_vector.at(i).port.toInt());
        send_socks.push_back(new_send_sock);
        friends_vector[i].sock_num = send_socks.length()-1;
        rstream.setDevice(send_socks.at(friends_vector.at(i).sock_num));
        rstream << Local_options->user_ip;
        rstream << " : ";
        rstream << sending_message->text();
        rstream << " end";
    }
    else
    {
        rstream.setDevice(send_socks.at(friends_vector.at(i).sock_num));
        rstream << Local_options->user_ip;
        rstream << " : ";
        rstream << sending_message->text();
        rstream << " end";
    }
    sending_message->clear();
}

void chat_menu::restart_serv()
{
    rserver->close();
    rserver->listen(QHostAddress(Local_options->user_ip), Local_options->user_port.toInt());
}

void chat_menu::keyPressEvent(QKeyEvent *key)
{
    if(key->text() == "\r")
        new_send_message();
}


