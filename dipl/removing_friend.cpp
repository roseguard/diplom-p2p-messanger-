#include "removing_friend.h"

removing_friend::removing_friend(QWidget *parent)
    : QWidget(parent)
{
    main_lay = new QVBoxLayout(this);
    friends_list = new QComboBox(this);
    remove = new QPushButton("Видалити обраного", this);
    exit = new QPushButton("Вийти", this);
    main_lay->addWidget(friends_list);
    main_lay->addWidget(remove);
    main_lay->addWidget(exit);
    db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("./db_usr");
    db->open();
    db->setPassword("roseguard");
    query = new QSqlQuery;
    if(db->tables().isEmpty()){
        query->exec("CREATE TABLE FRIENDS(name VARCHAR PRIMARY KEY,"
                   " ip VARCHAR, port VARCHAR, crypt VARCHAR);");
    }
    query = new QSqlQuery;
    update_list();
    connect(remove, SIGNAL(pressed()), this, SLOT(remove_from_db()));
    connect(exit, SIGNAL(pressed()), this, SLOT(close()));
}

removing_friend::~removing_friend(){
}

void removing_friend::update_list(){
    friends_list->clear();
    query = new QSqlQuery;
    query->exec("SELECT * FROM FRIENDS;");
    while(query->next()){
        friends_list->addItem(query->value(0).toString());
    }
}

void removing_friend::remove_from_db(){
    query = new QSqlQuery;
    QString for_request = "DELETE FROM FRIENDS where name == '";
    for_request.append(friends_list->currentText());
    for_request.append("';");
    qDebug() << for_request;
    qDebug() << query->exec(for_request);
    update_list();
}
