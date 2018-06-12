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
list< list<char> > letter_list;
vector< list<char> > letter_tmp_list;

std::vector< list<char> > one_file_list;
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

list<char> getIndex(int num, int max)
{
	list<char> lt;
	while( lt.size() != num)
	{
		int n = rand() % max;
        bool exist = false;
        for( auto it = lt.begin(); it != lt.end(); it++)
        {
            if(*it == l_s.at(n) )
            {
                exist = true;
                break;
            }
        }
        if(exist == false)
            lt.push_back(l_s.at(n));
	}
    //lt.sort();
    return lt;
}

void AddOne(int total, list<char> lt,list<char> lt_sorted)
{
    //cout << "add one .. .. " << letter_list.size() << " - "<< total  << " - "<< std::this_thread::get_id() << endl; ;
    std::lock_guard<std::mutex> l(_lock);
    letter_tmp_list.push_back(lt);
    if( nCur >= total)
        return;

    bool exist = false;
    for( auto it = letter_list.begin(); it != letter_list.end(); it++)
        {
            if(*it == lt_sorted)
            {
                exist = true;
                break;
            }
        }
        

    //cout << "add one .. .. 1 ";
        if( exist == false)
            for( auto it = letter_tmp_list.begin(); it != letter_tmp_list.end() ; it++)
            {
                if(*it == lt_sorted)
                {
                   letter_list.push_back(lt_sorted);
                   one_file_list.push_back(lt);
                   //cout << nCur ;

                    break;
                }
            }

    for( auto it = letter_tmp_list.cbegin(); it != letter_tmp_list.cend() && letter_tmp_list.size() !=0; it++)
    {
        if(*it == lt_sorted)
        {
            letter_tmp_list.erase(it);
        }
    }

    nCur++;
}

void doWork(int total)
{

    bool exist = false;
    while( nCur < total)
    {
        exist = false;
        auto lt = getIndex(20,31);

        auto lt_tmp = lt;
        lt_tmp.sort();

        for( auto it = letter_list.begin(); it != letter_list.end(); it++)
        {
            if(*it == lt_tmp)
            {
                exist = true;
                break;
            }
        }
        if(exist == false)
        {
            letter_tmp_list.push_back(lt_tmp);
            AddOne(total, lt,lt_tmp);
        }
        else
            continue;
    }
}

void createOne(int n)
{
    srand (time(NULL));
    nCur = 0;
    outdata.open("output/file_" + to_string(n) + ".csv", ios::app);
    cout << " start file_" + to_string(n) + ".csv" << endl;

    vector<thread> lt_thread;

    for(int n=0; n< 5; n++)
    {
        lt_thread.push_back( thread(doWork,10));
    }
    for (auto& th : lt_thread) th.join();

//write to file
    for(auto it = one_file_list.begin(); it != one_file_list.end(); it++)
    {
        auto lt = *it;
        for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
       {
          outdata <<  l_s.at(*it1) << ",";
       }
       outdata<<endl;
    }
        /*
           for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
           {
              outdata <<  l_s.at(*it1) << ",";
           }
           outdata<<endl;
*/
    outdata.close();
    cout << " end file_" + to_string(n) + ".csv" << endl;
}

int main()
{
    for(int n = 0; n< 10; n++)
    {
        std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
        letter_tmp_list.clear();
        createOne(n+1);
        std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
    }
    cout << "Total = " <<  letter_list.size() << endl;

    int ss[31] = {0};
    for( auto it = letter_list.begin(); it != letter_list.end(); it++)
    {
       auto lt = *it;
       for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
       {
          //cout << *it1 << "  ";
          ss[*it1] +=1;
       }
       //cout << endl;
    }
    int mT = 0;
    for(int n =0;n< 31; n++)
    {
        cout << n << " = " << ss[n] << endl;
        mT += ss[n];
    }

    cout << mT << endl;
	return 0;
}
