#include <iostream>
#include <thread>
#include <future>

using namespace std;

void fun()
{
	int n =0;
	while(n < 10)
	{
		//sleep(2);
		std::cout<<"hello fun. " << this_thread::get_id()<<endl;
		n++;
		std::this_thread::sleep_for(1s);
	}
	std::cout<<"hello fun" << this_thread::get_id()<<endl;
}

void fun2()
{
	int n =0;
	while(n < 6)
	{
		//sleep(2);
		std::cout<<"hello fun2. " << this_thread::get_id()<<endl;
		n++;
		std::this_thread::sleep_for(2s);
	}
	std::cout<<"hello fun2. " << this_thread::get_id()<<endl;
}

int main(int argc, char const *argv[]) {


    //thread t( fun2);
    

    thread t2(fun);
    

    auto f =async( fun2 );
    //t.join();
    //t2.join();
    //f.wait();

    std::cout<<"end"<<endl;

    while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */


    std::future<int> f1 = std::async(std::launch::async, [](){
        return 8;
    });

    cout << f1.get() << endl; //output: 8

    std::future<void> f2 = std::async(std::launch::async, [](){
        cout << 8 << endl;
        //return 8;
    });

    f2.wait(); //output: 8

    std::future<int> future = std::async(std::launch::async, [](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    });

    std::cout << "waiting...\n";
    
    //Test12();
    std::future_status status;
    std::this_thread::sleep_for(30s);
    do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            std::cout << "deferred\n";
        }
        else if (status == std::future_status::timeout) {
            std::cout << "timeout\n";
        }
        else if (status == std::future_status::ready) {
            std::cout << "ready!\n";
        }
    } while (status != std::future_status::ready);

    std::cout << "result is " << future.get() << '\n';



    return 0;
}



