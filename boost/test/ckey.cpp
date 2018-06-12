#include <iostream>
#include <string>

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>


using namespace std;

int main(int argc, char** argv )
{
	if(argc ==1)
	{
		cout  << "error" << endl;
		return 0;
	}
	 string s = argv[1];
cout  << s << endl;


CGEventRef event1, event2;
event1 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)6, true);//'z' keydown event
CGEventSetFlags(event1, kCGEventFlagMaskShift);//set shift key down for above event
CGEventPost(kCGSessionEventTap, event1);//post event


event2 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)6, false);
CGEventSetFlags(event2, kCGEventFlagMaskShift);
CGEventPost(kCGSessionEventTap, event2);


	return 0;
}