#ifndef REMOVING_FRIEND_H
#define REMOVING_FRIEND_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QDebug>

struct db_list_local
{
    QString name;
    QString ip;
    QString port;
    QString crypt;
};

class removing_friend : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout *main_lay;
    QComboBox *friends_list;
    QPushButton *remove;
    QPushButton *exit;

    QVector<struct db_list_local> friends_vector;

    QSqlDatabase *db;
    QSqlQuery *query;

public:
    void update_list();

public slots:
    void remove_from_db();

public:
    removing_friend(QWidget *parent = 0);
    ~removing_friend();
};

#endif // REMOVING_FRIEND_H
