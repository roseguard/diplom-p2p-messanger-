#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    main_lay = new QVBoxLayout(this);
    log = new QTextBrowser(this);
    exit = new QPushButton(this);
    exit->setText("exit");
    log->setReadOnly(true);
    log->append("staring server");
    main_lay->addWidget(log);
    main_lay->addWidget(exit);

    connect(exit, SIGNAL(pressed()), this, SLOT(close()));

    db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("/home/arhcitect/build-chat_server-Desktop_Qt_5_5_1_GCC_64bit-Debug/db");
    if(db->open())
        log->append("database connected");
    else
        log->append("error by opening database");

    for(int i = 0; db->tables().isEmpty() && i < 5; i++)
    {
        log->append("\nno tables in database");
        log->append("trying to create table");
        query = new QSqlQuery;
        log->append(QString(query->exec("CREATE TABLE users(name VARCHAR PRIMARY KEY, password VARCHAR, port VARCHAR, cript VARCHAR, ip VARCHAR);")));
        query->exec("insert into users(name,password,port,cript,ip) values('a','a','a','a','a');");
    }
    if(!db->tables().isEmpty())
    {
        log->append("database is enable");
        log->append("\nstarting server");

        rserver = new QTcpServer(this);
        connect(rserver, SIGNAL(newConnection()), this, SLOT(new_active_user()));
        rserver->listen(QHostAddress::Any, 21034);
        log->append("\nstarting listen\naddress : any\nport : 21034");
    }
}

Widget::~Widget()
{
    delete db;
}

void Widget::new_active_user()
{
    active_users.append(rserver->nextPendingConnection());
    QTcpSocket *for_test = for_test = active_users.last();
    connect(for_test, SIGNAL(readyRead()), this, SLOT(new_message()));
}

void Widget::new_message()
{
    log->append("\nnew message from socket");
    QString tempstr;
    QVector<QString>datas;
    QTextStream rstream;
    int i;
    log->append("searching socket");
    for(i = 0; i < active_users.size(); i++)
    {
        rstream.setDevice(active_users.at(i));
        rstream >> tempstr;
        if(tempstr.isEmpty())
            continue;
        else
            break;
    }
    log->append("reading all incoming datas");
    while(tempstr!="" && tempstr!="end")
    {
        datas.push_back(tempstr);
        rstream >> tempstr;
    }

    qDebug() << datas.size() << datas.length();
    qDebug() << datas;

    log->append(QString::number(active_users.at(i)->socketDescriptor()));

    if(datas.at(0) == "GETIP")
    {
        if(datas.size() < 2)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        query = new QSqlQuery;
        QString request = "select * from USERS where name = '";
        request.append(datas.at(1));
        request.append("';\0");
        log->append("request formed");
        query->finish();
        log->append("searching ip");
        query->exec(request);

        if(query->next())
        {
            QString rose = query->value(ip_num).toString();
            log->append("found ip");
            log->append(rose);
            log->append("sending ip");
            rose.append("\n");
            rstream << rose;
            rstream.flush();
            log->append("ip sended\nclosing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
        else
        {
            log->append("ip not found");
            log->append("sending error");
            rstream << "ERROR\n";
            rstream.flush();
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "GETPORT")
    {
        if(datas.size() < 2)
        {
            log->append("not enough datas");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        query = new QSqlQuery;
        QString request = "select * from USERS where name = '";
        request.append(datas.at(1));
        request.append("';\0");
        log->append("request formed");
        query->finish();
        log->append("searching port");
        query->exec(request);

        if(query->next())
        {
            QString rose = query->value(port_num).toString();
            log->append("found port");
            log->append(rose);
            log->append("sending port");
            rose.append("\n");
            rstream << rose;
            rstream.flush();
            log->append("port sended\nclosing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
        else
        {
            log->append("port not found");
            log->append("sending error");
            rstream << "ERROR\n";
            rstream.flush();
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "GETNAME")
    {
        if(datas.size() < 2)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        query = new QSqlQuery;
        QString request = "select * from USERS where ip = '";
        request.append(datas.at(1));
        request.append("';\0");
        log->append("request formed");
        query->finish();
        log->append("searching name");
        query->exec(request);

        if(query->next())
        {
            QString rose = query->value(name_num).toString();
            log->append("found name");
            log->append(rose);
            log->append("sending name");
            rose.append("\n");
            rstream << rose;
            rstream.flush();
            log->append("name sended\nclosing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "ERROR\n";
            rstream.flush();
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "GETCRIPT")
    {
        if(datas.size() < 2)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        query = new QSqlQuery;
        QString request = "select * from USERS where name = '";
        request.append(datas.at(1));
        request.append("';\0");
        log->append("request formed");
        query->finish();
        log->append("searching cript");
        query->exec(request);

        if(query->next())
        {
            QString rose = query->value(cript_num).toString();
            log->append("found cript");
            log->append(rose);
            log->append("sending cript");
            rose.append("\n");
            rstream << rose;
            rstream.flush();
            log->append("cript sended\nclosing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
        else
        {
            log->append("cript not found");
            log->append("sending error");
            rstream << "ERROR\n";
            rstream.flush();
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "REGUSER")
    {
        if(datas.size() < 6)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        log->append("user asking for register");
        query = new QSqlQuery;
        QString for_request = ("insert into users(name,password,port,cript,ip) values(");
        for(int l = 0; l < 5; l++)
        {
            for_request.append("'");
            if(l == 0)
            {
                log->append("checking if name already register");
                QString temp_req = "select * from users where name == '";
                temp_req.append(datas.at(1));
                temp_req.append("';");
                query->exec(temp_req);
                if(query->next())
                {
                    log->append("name already register");
                    log->append("informing user");
                    rstream << "ALRDREGNM";
                    log->append("register failed\nclosing socket");
                    active_users.at(i)->close();
                    active_users.removeAt(i);
                    return;
                }
                log->append("no user with this name in database\ncontinue registering");
            }
            for_request.append(datas.at(l+1));
            for_request.append("'");
            if(l!=4)
                for_request.append(",");
        }
        rstream << "SUCCES\n";
        for_request.append(");");
        query->exec(for_request);
        log->append("user registered");
        log->append("closing socket");
        active_users.at(i)->close();
        active_users.removeAt(i);
    }
    else if(datas.at(0) == "LOGUSER")
    {
        if(datas.size() < 3)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        log->append("user asking for log");
        query = new QSqlQuery;
        QString for_request = ("SELECT * FROM USERS where name == '");
        for_request.append(datas.at(1));
        for_request.append("';");
        query->exec(for_request);
        log->append("searching name in database");
        if(query->next())
        {
            log->append("name found");
            QString for_pass = query->value(1).toString();
            log->append("cheking pass");
            if(datas.at(2) == for_pass)
            {
                log->append("password - correct");
                rstream << "SUCCES\n";
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
            else
            {
                log->append("pass = incorrect");
                log->append("sending error");
                rstream << "INCORRECT_PASS";
                log->append("log canceled");
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "INCORRECT_NAME";
            log->append("login canceled");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "SETpass")
    {
        if(datas.size() < 4)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        log->append("user asking for changing pass");
        query = new QSqlQuery;
        QString for_request = ("SELECT * FROM USERS where name == '");
        for_request.append(datas.at(1));
        for_request.append("';");
        query->exec(for_request);
        log->append("searching name in database");
        if(query->next())
        {
            log->append("name found");
            QString for_pass = query->value(1).toString();
            log->append("cheking pass");
            if(datas.at(2) == for_pass)
            {
                log->append("password - correct");
                log->append("changing pass");
                for_request = "UPDATE users SET password='";
                for_request.append(datas.at(3));
                for_request.append("' WHERE name == '");
                for_request.append(datas.at(1));
                for_request.append("';");
                query->clear();
                query->exec(for_request);
                qDebug() << for_request;
                qDebug() << for_pass;
                log->append("password changed. Informing user");
                rstream << "SUCCES";
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
            else
            {
                log->append("pass = incorrect");
                log->append("sending error");
                rstream << "INCORRECT_PASS";
                log->append("changing canceled");
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "INCORRECT_NAME";
            log->append("changing canceled");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "SETip")
    {
        if(datas.size() < 4)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        log->append("user asking for changing ip");
        query = new QSqlQuery;
        QString for_request = ("SELECT * FROM USERS where name == '");
        for_request.append(datas.at(1));
        for_request.append("';");
        query->exec(for_request);
        log->append("searching name in database");
        if(query->next())
        {
            log->append("name found");
            QString for_pass = query->value(1).toString();
            log->append("cheking pass");
            if(datas.at(2) == for_pass)
            {
                log->append("password - correct");
                log->append("changing ip");
                for_request = "UPDATE users SET ip='";
                for_request.append(datas.at(3));
                for_request.append("' WHERE name == '");
                for_request.append(datas.at(1));
                for_request.append("';");
                query->clear();
                query->exec(for_request);
                qDebug() << for_request;
                qDebug() << for_pass;
                log->append("ip changed. Informing user");
                rstream << "SUCCES";
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
            else
            {
                log->append("pass = incorrect");
                log->append("sending error");
                rstream << "INCORRECT_PASS";
                log->append("changing canceled");
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "INCORRECT_NAME";
            log->append("login canceled");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "SETport")
    {
        log->append("user asking for changing port");
        query = new QSqlQuery;
        QString for_request = ("SELECT * FROM USERS where name == '");
        for_request.append(datas.at(1));
        for_request.append("';");
        query->exec(for_request);
        log->append("searching name in database");
        if(query->next())
        {
            log->append("name found");
            QString for_pass = query->value(1).toString();
            log->append("cheking pass");
            if(datas.at(2) == for_pass)
            {
                log->append("password - correct");
                log->append("changing port");
                for_request = "UPDATE users SET port='";
                for_request.append(datas.at(3));
                for_request.append("' WHERE name == '");
                for_request.append(datas.at(1));
                for_request.append("';");
                query->clear();
                query->exec(for_request);
                qDebug() << for_request;
                qDebug() << for_pass;
                log->append("port changed. Informing user");
                rstream << "SUCCES";
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
            else
            {
                log->append("pass = incorrect");
                log->append("sending error");
                rstream << "INCORRECT_PASS";
                log->append("changing canceled");
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "INCORRECT_NAME";
            log->append("login canceled");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
    else if(datas.at(0) == "SETcrypt")
    {
        if(datas.size() < 4)
        {
            log->append("not enough datas");
            log->append("closing socket");
            rstream << "ERROR";
            active_users.at(i)->close();
            active_users.removeAt(i);
            return;
        }
        log->append("user asking for updating port");
        query = new QSqlQuery;
        QString for_request = ("SELECT * FROM USERS where name == '");
        for_request.append(datas.at(1));
        for_request.append("';");
        query->exec(for_request);
        log->append("searching name in database");
        if(query->next())
        {
            log->append("name found");
            QString for_pass = query->value(1).toString();
            log->append("cheking pass");
            if(datas.at(2) == for_pass)
            {
                log->append("password - correct");
                log->append("updating crypt");
                for_request = "UPDATE users SET crypt='";
                for_request.append(datas.at(3));
                for_request.append("' WHERE name == '");
                for_request.append(datas.at(1));
                for_request.append("';");
                query->clear();
                query->exec(for_request);
                qDebug() << for_request;
                qDebug() << for_pass;
                log->append("crypt updated. Informing user");
                rstream << "SUCCES";
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
            else
            {
                log->append("pass = incorrect");
                log->append("sending error");
                rstream << "INCORRECT_PASS";
                log->append("changing canceled");
                log->append("closing socket");
                active_users.at(i)->close();
                active_users.removeAt(i);
            }
        }
        else
        {
            log->append("name not found");
            log->append("sending error");
            rstream << "INCORRECT_NAME";
            log->append("login canceled");
            log->append("closing socket");
            active_users.at(i)->close();
            active_users.removeAt(i);
        }
    }
}
