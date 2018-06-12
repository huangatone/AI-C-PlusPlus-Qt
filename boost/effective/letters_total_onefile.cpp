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
        auto v1_it = find (lt.begin(), lt.end(), n);
        if (v1_it == lt.end())
        {
            lt.push_back(n);
        }
        
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
      
        if(letter_list.size() >= total)
            break;
       auto v1_it = find (letter_list.begin(), letter_list.end(), lt);
        if (v1_it == letter_list.end())
        {
            letter_list.push_back(lt);
            nCur++;
        }
    }
}



void oneJob(int index, int total)
{
    srand (time(NULL));
    outdata.open( to_string(index) +  "_aaron.csv", ios::app);

    vector<thread> lt_thread;
    std::cout << "Job "  <<  to_string(index) << "  currentDateTime()=" << currentDateTime() << std::endl;
    cout << "Total = " << total << endl;
    doWork(total);
   

    cout << letter_list.size() <<"--" << nCur <<  endl;

    string one_line_data; 
    for( auto it = letter_list.begin(); it != letter_list.end(); it++)
    {
       auto lt = *it;
       one_line_data = "";
       for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
       {
          one_line_data.push_back(l_s.at(*it1));
          one_line_data.push_back(  ',');
       }
       outdata << one_line_data <<endl;

    }
    outdata.close();
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
}


int main(int num, char *argv[])
{
    int nTotal = 1000;
    int nFile = 1;
    if(num >=2)
        nTotal = atoi(argv[1]);
    //if(num >=3)
    //    nFile = atoi( argv[2]);

    letter_list.clear();
    for(int n=0; n< nFile; n++)
        oneJob(n+1,nTotal);
	return 0;
}
