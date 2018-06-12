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
#include <time.h>
#include <fstream>

using namespace std;

condition_variable _task_cv;   //条件阻塞
mutex _lock;                   //同步
list< list<int> > letter_list;
 int nCur =0;

 vector<char> l_s = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
               'U','V','W','X','Y','Z','1','2','3','4','5'};
 ofstream outdata;


 // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
 const std::string currentDateTime() {
     time_t     now = time(0);
     struct tm  tstruct;
     char       buf[80];
     tstruct = *localtime(&now);
     // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
     // for more information about date/time format
     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

     return buf;
 }
list<int> getIndex(int num, int max)
{
	list<int> lt;
	while( lt.size() != num)
	{
		int n = rand() % max;
        bool exist = false;
        for( auto it = lt.begin(); it != lt.end(); it++)
        {
            if(*it == n)
            {
                exist = true;
                break;
            }
        }
        if(exist == false)
            lt.push_back(n);
	}
    lt.sort();
    return lt;
}



void doWork(int total)
{

    bool exist = false;
    while( letter_list.size() < total)
    {
        exist = false;
        auto lt = getIndex(20,31);
        std::lock_guard<std::mutex> l(_lock);
        if(letter_list.size() >= total)
            break;
        for( auto it = letter_list.begin(); it != letter_list.end(); it++)
        {
            if(*it == lt)
            {
                exist = true;
                break;
            }
        }
        if(exist == false)
            letter_list.push_back(lt);
        nCur++;
    }
}



int main()
{
	srand (time(NULL));
    outdata.open("aaron.csv", ios::app);

	vector<thread> lt_thread;
 std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
	for(int n=0; n< 3; n++)
	{
        lt_thread.push_back( thread(doWork,10000));
	}
	for (auto& th : lt_thread) th.join();

    cout << letter_list.size() <<"--" << nCur <<  endl;

    for( auto it = letter_list.begin(); it != letter_list.end(); it++)
    {
       auto lt = *it;
       for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
       {
          outdata <<  l_s.at(*it1) << ",";
       }
       outdata<<endl;

    }
    outdata.close();
 std::cout << "currentDateTime()=" << currentDateTime() << std::endl;

	return 0;
}
