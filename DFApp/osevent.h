#ifndef OSEVENT_H
#define OSEVENT_H

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>

class osevent
{
public:
	osevent();
};

CGKeyCode QtKeyCode2MacKeyCode(char k);
bool getFlags(char k);
void click(int x, int y);
void dbclick(int x, int y);
void key(char ch);

#endif // OSEVENT_H
