#ifndef ADDING_FRIEND_H
#define ADDING_FRIEND_H

#include <QtNetwork/QTcpSocket>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QLabel>
#include <QComboBox>
#include <QTextStream>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QHostAddress>
#include <QMessageBox>

class adding_friend : public QWidget
{
    Q_OBJECT
public:
    adding_friend(QWidget *parent = 0);
    ~adding_friend();
    QVBoxLayout *main_lay;
    QHBoxLayout *radio_butt;
    QRadioButton *by_name;
    QRadioButton *by_local;
    QHBoxLayout *serv_lay;
    QLabel *serv_lab;
    QLineEdit *serv_edit;
    QHBoxLayout *servp_lay;
    QLabel *servp_lab;
    QLineEdit *servp_edit;
    QHBoxLayout *by_name_lay;
    QLabel *name_lab;
    QLineEdit *name_edit;
    QVBoxLayout *by_local_lay;
    QHBoxLayout *local_name_lay;
    QLabel *local_name_lab;
    QLineEdit *local_name_edit;
    QHBoxLayout *local_ip_lay;
    QLabel *local_ip_lab;
    QLineEdit *local_ip_edit;
    QHBoxLayout *local_port_lay;
    QLabel *local_port_lab;
    QLineEdit *local_port_edit;
    QHBoxLayout *local_crypt_lay;
    QLabel *local_crypt_lab;
    QLineEdit *local_crypt_edit;
    QHBoxLayout *butts;
    QPushButton *search;
    QPushButton *cancel;
    QPushButton *update;
    QComboBox *from_parent;
    QTcpSocket *sender;
    QSqlDatabase *database;
    QSqlQuery *query;
    QMessageBox *mess;
    QString rname, rip, rport, rcrypt;
    QString for_send[3];
    int send_num = 0;
    int name_num = 0;
    void flush_to_db(bool update);
    void debug_db();
public slots:
    void change_type();
    void searching();
    void getting();
    void updating_start();
    void get_data();
};
#endif // ADDING_FRIEND_H
