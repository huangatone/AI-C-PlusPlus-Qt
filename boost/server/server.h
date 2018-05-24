#ifndef __CLY_SERVER_H__
#define __CLY_SERVER_H__
#include <string.h>  
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
class Session;
class Server {
private:
	// 会话 - 智能指针
	typedef	boost::shared_ptr<Session>	session_ptr;
public:
	Server(boost::asio::io_service &_ioService, boost::asio::ip::tcp::endpoint &_endpoint);
	virtual ~Server(void);
	// 监听
	void start(void);
	// 异步
	void run(void);
private:
	// 数据导出接口
	void callback_session(std::string _fromIp, std::string _info);
	// 会话启动
	void accept_handler(session_ptr _chatSession, const boost::system::error_code& _error);
private:
	boost::asio::io_service &ioService_;
	boost::asio::ip::tcp::acceptor acceptor_;
};
#endif // __CLY_SERVER_H__