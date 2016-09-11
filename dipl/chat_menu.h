// класс обміну повідомлень.

#ifndef CHAT_MENU_H
#define CHAT_MENU_H

#include "options.h"
#include "adding_friend.h"
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
#include <QComboBox>
#include <QListView>
#include "widget.h"
#include <QTextBrowser>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

class chat_menu : public QWidget
{
    Q_OBJECT

struct db_list
{
    QString name;
    QString ip;
    QString port;
    QString crypt;
    int sock_num;
};
public:
    Options *Local_options;
    char *message;
    options *A;
    QHBoxLayout *main_lay;
    QVBoxLayout *message_lay;
    QTextBrowser *messages;
    QVBoxLayout *friends_lay;
    QPushButton *add_friend;
    QPushButton *remove_friend;
    QPushButton *change_options;
    QPushButton *exit;
    QHBoxLayout *sending_message_lay;
    QLineEdit *sending_message;
    QPushButton *send_message;
    QComboBox *friends_list;
    adding_friend *ad;
    QVector <struct db_list> friends_vector;
    QSqlDatabase *db;
    QSqlQuery *query;
    QTimer *list_timer;
    QTcpServer* rserver;
    QVector<QTcpSocket*> recv_socks;
    QVector<QTcpSocket*> send_socks;
    bool _server = 0;
    int combo_num = 1;
    void keyPressEvent(QKeyEvent* key);
public slots:
            void changing_options();
            void f_adding_friend();
            void f_removing_friend();
            void end_chat();
            void update_list();
            void new_connection();
            void new_recv_message();
            void new_send_message();
            void disconnect_all();
            void restart_serv();
signals:
            void new_message();
public:
    explicit chat_menu(Options* _opts , bool server = 0, QWidget *parent = 0);
    ~chat_menu();
};

#endif // CHAT_MENU_H
