#ifndef CHANGE_INFO_H
#define CHANGE_INFO_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QTextStream>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <string>
#include <QCryptographicHash>
class change_info : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout* main_lay;
    QHBoxLayout *serv_lay;
    QLabel *serv_lab;
    QLineEdit *serv_edit;
    QHBoxLayout *servp_lay;
    QLabel *servp_lab;
    QLineEdit *servp_edit;
    QHBoxLayout* old_name_lay;
    QLabel *old_name_lab;
    QLineEdit *old_name_edit;
    QHBoxLayout* old_pass_lay;
    QLabel *old_pass_lab;
    QLineEdit *old_pass_edit;
    QHBoxLayout* name_lay;
    QLabel *name_lab;
    QLineEdit *name_edit;
    QHBoxLayout* pass_lay;
    QLabel *pass_lab;
    QLineEdit *pass_edit;
    QHBoxLayout* port_lay;
    QLabel *port_lab;
    QLineEdit *port_edit;
    QHBoxLayout* ip_lay;
    QLabel *ip_lab;
    QLineEdit *ip_edit;
    QPushButton *chbutt;
    QTcpSocket *regsock;
    QMessageBox *mess;
    QString for_change;
    QString open_key;
private:
    void gen_public_key();
public slots:
    void sending_changing();
    void getting_answer();
public:
    change_info(QString _for_change = "crypt", QWidget *parent = 0);
    ~change_info();
};
#endif // CHANGE_INFO_H
