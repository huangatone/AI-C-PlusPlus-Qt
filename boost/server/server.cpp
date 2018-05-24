#include <boost/bind.hpp>

#include "server.h"
#include "session.h"


Server::Server(boost::asio::io_service &_ioService, boost::asio::ip::tcp::endpoint &_endpoint)
: ioService_(_ioService), acceptor_(_ioService, _endpoint) {
	start();
}


Server::~Server(void) {
}

void Server::start(void) {
	session_ptr	new_chat_session(new Session(ioService_));

	acceptor_.async_accept(new_chat_session->socket(),
		boost::bind(&Server::accept_handler, this, new_chat_session,
		boost::asio::placeholders::error));
}

void Server::run(void) {
	ioService_.run();
}

void Server::callback_session(std::string /*_fromIp*/, std::string /*_info*/) {
	return;
}

void Server::accept_handler(session_ptr _chatSession, const boost::system::error_code& _error) {
	if (!_error && _chatSession) {
		try {
			_chatSession->start();
			start();
		}
		catch (...) {
			return;
		}
	}
}