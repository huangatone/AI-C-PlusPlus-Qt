#include <iostream>
using namespace std;
#include <boost/asio.hpp>
using namespace boost::asio;

int main()
{
    boost::asio::io_service io_service;
    boost::system::error_code error;  
    boost::asio::ip::tcp::socket sock(io_service);
    boost::asio::ip::tcp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), 8585);

    sock.connect(local_add,error);
    string msg = "connected to 127.0.0.1"; 
    sock.write_some(buffer(msg));

    char receive_str[1024] = { 0 };//字符串
    while (1)
    {
        boost::asio::ip::tcp::endpoint  sendpoint;//请求的IP以及端口
        char buf[128];  
            size_t len = sock.read_some(boost::asio::buffer(buf), error);  

        // 这是也要判断一下, 否则服务端运行断开, 这里会出现死循环.  
            if (error == boost::asio::error::eof)  
                break; // Connection closed cleanly by peer.  
            else if (error)  
                throw boost::system::system_error(error); // Some other error.  
             std::cout << "recv succeed: " << buf << std::endl; 
            // buf要注意控制大小。  
            sock.write_some(boost::asio::buffer(msg, 10), error);  
        
     
    }

    return 0;
}