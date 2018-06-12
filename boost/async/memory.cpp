
#include <iostream>
#include <list>
#include <string>

using namespace std;
// compile with: /LD  
struct Date1 {  
   unsigned nWeekDay  : 3;    // 0..7   (3 bits)  
   unsigned nMonthDay : 6;    // 0..31  (6 bits)  
  // unsigned           : 0;    // Force alignment to next boundary.  
   unsigned nMonth    : 5;    // 0..12  (5 bits)  
   unsigned nYear     : 8;    // 0..100 (8 bits)  
};  

struct Date2 {  
   unsigned nWeekDay  ;    // 0..7   (3 bits)  
   unsigned nMonthDay ;    // 0..31  (6 bits)  
  // unsigned           : 0;    // Force alignment to next boundary.  
   unsigned nMonth    ;    // 0..12  (5 bits)  
   unsigned nYear     ;    // 0..100 (8 bits)  
};  

struct box_props
{
     unsigned int opaque       : 1;
     unsigned int fill_color   : 3;
     unsigned int bg_color   : 3;
 };


int main()
{
	auto a = sizeof(Date1);

	cout << a << " - " << sizeof(Date2) << endl;

	cout << sizeof(box_props) << endl;

	box_props b;
	b.opaque = 1;
	b.fill_color = 3;
	b.bg_color = 2;

	cout << b.opaque << " - " << b.fill_color << endl;

	printf("char size = %d\n", sizeof(char));
    printf("unsigned char size = %d\n\n", sizeof(unsigned char));

    printf("short size = %d\n", sizeof(short));
    printf("unsigned short size = %d\n\n", sizeof(unsigned short));

    printf("int size = %d\n", sizeof(int));
    printf("unsigned int size = %d\n\n", sizeof(unsigned int));
}