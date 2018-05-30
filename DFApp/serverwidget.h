#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>

#include <QTcpServer>
#include <QList>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ServerWidget(QWidget *parent = 0);
	~ServerWidget();
protected slots:
	void slot_newConnection();
	void client_readyRead();
	void slot_disconnected();
private slots:
	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

private:
	Ui::ServerWidget *ui;
	QTcpServer* _server_socket;
	QList<QTcpSocket*> _lt_client_sock;
};

#endif // SERVERWIDGET_H
