#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //socket_tcp.readyRead();

    QObject::connect(&socket_ssl, &QSslSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_ssl, &QSslSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_ssl, &QSslSocket::readyRead, this, &Widget::doReadyRead);

    QObject::connect(&socket_tcp, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_tcp, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_tcp, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);


    changeEnable();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEnable(){
    ui->pbConnect->setEnabled(!flag_connect);
    ui->pbDisconnect->setEnabled(flag_connect);
}

void Widget::doConnected() {
    ui->pteMessage->insertPlainText("Connected \r\n");

    flag_connect = true;
    changeEnable();
}

void Widget::doDisconnected() {
    ui->pteMessage->insertPlainText("Disconnected");

    flag_connect = false;
    changeEnable();
}

void Widget::doReadyRead() {
    if(ui->cbMode->isChecked()) {
        ui->pteMessage->insertPlainText(socket_ssl.readAll());
    } else {
        ui->pteMessage->insertPlainText(socket_tcp.readAll());
    }

}

void Widget::on_pbConnect_clicked()
{
    if(ui->cbMode->isChecked()) {
        socket_ssl.connectToHostEncrypted(ui->leHost->text(),ui->lePort->text().toUShort());
    } else {
        socket_tcp.connectToHost(ui->leHost->text(),ui->lePort->text().toUShort());
    }
}


void Widget::on_pbDisconnect_clicked()
{
    if(ui->cbMode->isChecked()) {
        socket_ssl.close();
    } else {
        socket_tcp.close();
    }

}


void Widget::on_pbSend_clicked()
{
    if(ui->cbMode->isChecked()) {
        socket_ssl.write(ui->pteSend->toPlainText().toUtf8());
    } else {
        socket_tcp.write(ui->pteSend->toPlainText().toUtf8());
    }

}


void Widget::on_cbMode_stateChanged(int arg1)
{

}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

