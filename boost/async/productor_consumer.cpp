#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <thread>
#include <future>
//#include <condition_variable>
//#include <thread>
//#include <functional>
#include <stdexcept>
#include <iostream>
#include <list>
#include <string>

using namespace std;

condition_variable _task_cv;   //条件阻塞
mutex _lock;                   //同步

list<string> msg_lst;

void consumer_fun()
{
	while(true)
	{
		int n = rand() % 2000;
		std::this_thread::sleep_for( std::chrono::microseconds(n) ); 
		//cout << "wait ---: " << std::this_thread::get_id()<< endl;
		unique_lock<mutex> lock{ _lock };
		_task_cv.wait(lock, [] { return msg_lst.size() !=0;}); // wait 直到有 task
		cout <<std::this_thread::get_id()<< " read: " << msg_lst.size()  << endl;
		msg_lst.pop_front() ;
	}


}

void product_fun()
{	
    char buffer[256];
    while(true)
    {
    	int n = rand() % 4000;
        //get msg from ui
        //printf("Please enter the message: ");
        cout <<std::this_thread::get_id()  <<" put :" << msg_lst.size() << endl;
        //bzero(buffer,256);
        //fgets(buffer,255,stdin);
        unique_lock<mutex> lock{ _lock };
        msg_lst.push_back( to_string(n));
        _task_cv.notify_one(); // 唤醒一个线程执行

        std::this_thread::sleep_for( std::chrono::microseconds(n) );        
    }
    
}

int main()
{
	srand (time(NULL));
	msg_lst.push_back("A1");
	msg_lst.push_back("A2");
	msg_lst.push_back("A3");
	msg_lst.push_back("A4");
	msg_lst.push_back("A5");
	msg_lst.push_back("A6");
	vector<thread> lt_thread;

	for(int n=0; n< 3; n++)
	{
		lt_thread.push_back( thread(consumer_fun));
		lt_thread.push_back( thread(product_fun));
	}
	for(int n=0; n< 3; n++)
	{
		lt_thread.push_back( thread(consumer_fun));
		//lt_thread.push_back( thread(notice_fun));
	}

	//thread t2(notice_fun);

	for (auto& th : lt_thread) th.join();
	//t2.join();
	return 0;
}