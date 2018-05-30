#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QTcpSocket>

ServerWidget::ServerWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ServerWidget)
{
	ui->setupUi(this);
	_server_socket = new QTcpServer(this);
	connect(_server_socket, SIGNAL(newConnection()),this, SLOT(slot_newConnection()));
}

ServerWidget::~ServerWidget()
{
	delete ui;
}

void ServerWidget::slot_newConnection()
{
	if(_server_socket->hasPendingConnections())
	{
		auto sock = _server_socket->nextPendingConnection();
		connect(sock, SIGNAL(readyRead()),this, SLOT(client_readyRead()));
		ui->listWidget->addItem("Client:-  : " + sock->peerAddress().toString());
		_lt_client_sock.append(sock);
		ui->label_3->setText( "Current connection: " + QString::number( _lt_client_sock.size()));
	}
}

void ServerWidget::client_readyRead()
{
	QTcpSocket* sock = dynamic_cast<QTcpSocket*>( sender());
	auto msg = sock->readAll();
	ui->listWidget->addItem("Get:-  : " + msg + " from = " + QString::number(sock->peerPort()));
	sock->write("Got your", 10);

	if(ui->listWidget->count() > 1000)
		ui->listWidget->clear();
}

void ServerWidget::slot_disconnected()
{
	QTcpSocket* sock = dynamic_cast<QTcpSocket*>( sender());
	_lt_client_sock.removeOne(sock);
	ui->label_3->setText( "Current connection: " + QString::number( _lt_client_sock.size()));
}

void ServerWidget::on_toolButton_clicked()
{
	int pt = ui->lineEdit->text().toInt();
	bool s= _server_socket->listen( QHostAddress("127.0.0.1"), 8811);
	if(!s)
		ui->listWidget->addItem("Faild to start");
	else
		ui->listWidget->addItem("start to listen");

}

void ServerWidget::on_toolButton_2_clicked()
{
	//send to sock;
}
