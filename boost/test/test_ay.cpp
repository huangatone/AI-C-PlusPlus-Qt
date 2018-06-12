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

void aya_fun()
{
	while(true)
	{
		cout << "wait ---: " << std::this_thread::get_id()<< endl;
		unique_lock<mutex> lock{ _lock };
		_task_cv.wait(lock, [] { return msg_lst.size() !=0;}); // wait 直到有 task
		cout << "do work: " << msg_lst.front() << " -   --  -- "<<std::this_thread::get_id() << endl;
		msg_lst.pop_front() ;
	}


}

void notice_fun()
{	
    char buffer[256];
    while(true)
    {
    	int n = rand() % 500;
        //get msg from ui
        printf("Please enter the message: ");
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
		lt_thread.push_back( thread(aya_fun));
	}
	thread t2(notice_fun);

	for (auto& th : lt_thread) th.join();
	t2.join();
	return 0;
}