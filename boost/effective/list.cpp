// find example
#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector

using namespace std;

int main () {
  // using std::find with array and pointer:
  int myints[] = { 10, 20, 30, 40 };
  int * p;

  p = std::find (myints, myints+4, 30);
  if (p != myints+4)
    std::cout << "Element found in myints: " << *p << '\n';
  else
    std::cout << "Element not found in myints\n";

  // using std::find with vector and iterator:
  std::vector<int> myvector (myints,myints+4);
  std::vector<int>::iterator it;

  it = find (myvector.begin(), myvector.end(), 30);
  if (it != myvector.end())
    std::cout << "Element found in myvector: " << *it << '\n';
  else
    std::cout << "Element not found in myvector\n";

  vector < vector<int> > v1;
  v1.push_back( { 1,2,3} );
  v1.push_back( { 3,2,3} );
  v1.push_back( { 3,4,5} );
  v1.push_back( { 6,5,2} );
  v1.push_back( { 3,2,3} );

  vector < vector<int> >::iterator v1_it ;

  vector<int > f_v= { 3,4,5};
  v1_it = find (v1.begin(), v1.end(), f_v);
  if (v1_it != v1.end())
  {
    std::cout << "Element found in v1: "  << '\n';
    auto lt = *v1_it;
    for(auto ii = lt.begin(); ii != lt.end(); ii++ )
      cout << *ii << endl;

  }
  else
    std::cout << "Element not found in v1\n";

   f_v= { 3,2,3};

  int mycount = std::count (v1.begin(), v1.end(), f_v);
   std::cout << "f_v appears " << mycount << " times.\n";



  return 0;
}