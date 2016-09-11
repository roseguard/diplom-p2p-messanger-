// класс стартового вікна

#ifndef WIDGET_H
#define WIDGET_H

#include <QString>

#include <QWidget>
#include <QComboBox>
#include <QTextBlock>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QCheckBox>

struct Options
{
    QString user_name;
    QString user_port;
    QString user_ip;
};
class Widget : public QWidget
{
    Q_OBJECT
public:
    struct Options *User_options;
    QPushButton *start_with_login;
    QPushButton *start_without_login;
    QPushButton *options_butt;
    QPushButton *exit_butt;
    QVBoxLayout *lay;
public slots:
    void open_options();
    void start_chat();
    void start_login();
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

};

#endif // WIDGET_H
