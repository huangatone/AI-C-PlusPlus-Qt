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

#include <string>
#include <time.h>
#include <fstream>
#include <iomanip>
using namespace std;

condition_variable _task_cv;   //条件阻塞
mutex _lock;                   //同步
vector< vector<int> > letter_list;
 int nCur =0;

 vector<string> l_s = {"A","B","C","D","E","F","G","H","I","J","K", "L","M", "N","O","P","Q","R","S","T","U","V","W","X",
                        "Y","Z","1","2","3","4","5"};
 ofstream outdata;
struct tm  start_tm;
time_t     now ;
struct tm  tstruct;  

void printTime(int total)
 {
    while(true && (  letter_list.size() < total ) ) 
    {
        now = time(0);  
    tstruct = *localtime(&now);
    std::cout << '\r'
          << setw(2) << std::setfill('0') << tstruct.tm_hour - start_tm.tm_hour << ':'
          << std::setw(2) <<  tstruct.tm_min - start_tm.tm_min << ':'
          << std::setw(2) <<  tstruct.tm_sec - start_tm.tm_sec << " - " << letter_list.size()<< std::flush;

        std::this_thread::sleep_for( std::chrono::microseconds(1000*60) );  
    }   

 }
 // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
 const std::string currentDateTime() {
     
     struct tm  tstruct;
     char       buf[80];
     tstruct = *localtime(&now);
     // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
     // for more information about date/time format
     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

     return buf;
 }


vector<int> getIndex(int num, int max)
{
	vector<int> lt;
	while( lt.size() != num)
	{
		int n = rand() % max;
        auto v1_it = find (lt.begin(), lt.end(), n);
        if (v1_it == lt.end())
        {
            lt.push_back(n);
        }
        
	}
     std::sort(lt.begin(),lt.end());
    return lt;
}

bool find(vector<int> lt)
{

    for( auto it = letter_list.begin(); it != letter_list.end(); it++)
    {
        auto lt_cur = *it;
        bool bMatch = true;
        for(int n=0; n< lt.size(); n++)
        {
            if(lt.at(n) != lt_cur.at(n))
            {
                bMatch = false;
                break;
            }
            if(lt[n]< lt_cur[n])
                return false;
        }
        if(bMatch == true)
            return true;
    }
    return false;
}


void doWork(int total)
{
    std::cout << "Job "  <<  "  currentDateTime()=" << currentDateTime() << std::endl;
    cout << "Total = " << total << endl;
    srand (time(NULL));   
    while( nCur < total)
    {
        auto lt = getIndex(20,31);
      
        if(letter_list.size() >= total)
            break;
       auto v1_it = find (letter_list.begin(), letter_list.end(), lt);
        if (v1_it == letter_list.end())
        {
            letter_list.push_back(lt);
            //std::sort(letter_list.begin(),letter_list.end());
            nCur++;
        }
    }
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
}



void saveJob(int index, int total)
{
    
    outdata.open(  "file_nothread_" + to_string(index)  + ".csv", ios::app);   

    string one_line_data; 
    int start = index * total;


    vector< vector<int> >::iterator it1 = letter_list.begin();
    std::advance(it1, start);
    auto it2 = letter_list.begin();
    std::advance(it2, start+total);

    //list< list<int> > lt_one(letter_list+ start, letter_list+ (start+total));
    for( auto it = it1; it != it2; it++)
    {
       auto lt = *it;
       one_line_data = "";
       for( auto it1 = lt.begin(); it1 != lt.end(); it1++)
       {
          one_line_data+=   l_s.at(*it1)  + ",";
       }
       outdata << one_line_data <<endl;

    }
    
    outdata.close();
    
}


int main(int num, char *argv[])
{
  cout << " app total files threads";
    int nTotal = 1000;
    int nFile = 1;
    int nth = 1;
    if(num >=2)
        nTotal = atoi(argv[1]);
    if(num >=3)
        nFile = atoi( argv[2]);
     // if(num >=4)
     //   nth = atoi(argv[3]);

     now = time(0);  
     start_tm = *localtime(&now);

    letter_list.clear();
    thread th(printTime,nTotal);
   

    vector<thread> lt_thread;
    for(int n=0; n< nth; n++)
    {
        lt_thread.push_back( thread(doWork,nTotal));
    }
    for (auto& th : lt_thread) th.join();

    //generate total records;
    //doWork(nTotal);
 
    th.join();
    //save to files;
    int sun_total = nTotal/ nFile;
    for(int n=0; n< nFile; n++)
    {
        saveJob(n,sun_total);
    }

    //for(int n=0; n< nFile; n++)
    
	return 0;
}
