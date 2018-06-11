#include "osevent.h"
#include <QKeyEvent>
#include <QDebug>

osevent::osevent()
{

}


void click(int x, int y)
{
	CGPoint point;
	point.x = x;
	point.y = y;
	qDebug() <<"click at" <<  point.x << point.y;
	CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);
	theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);
}

void dbclick(int x, int y)
{
	qDebug() <<"dbclick at" <<  x << y;
	CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CGEventSetType(theEvent, kCGEventLeftMouseUp);
	CGEventPost(kCGHIDEventTap, theEvent);

	CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, 2);

	CGEventSetType(theEvent, kCGEventLeftMouseDown);
	CGEventPost(kCGHIDEventTap, theEvent);

	CGEventSetType(theEvent, kCGEventLeftMouseUp);
	CGEventPost(kCGHIDEventTap, theEvent);

	CFRelease(theEvent);

}


void key(char ch)
{
	CGKeyCode k = QtKeyCode2MacKeyCode(ch);
	CGEventRef mkey = CGEventCreateKeyboardEvent(NULL, k, true);
	if(getFlags(ch))
		CGEventSetFlags(mkey, kCGEventFlagMaskShift);//set shift key down for above event
	CGEventPost(kCGHIDEventTap, mkey);
	CFRelease(mkey);
}

CGKeyCode QtKeyCode2MacKeyCode(char k)
{
	switch(k){
	case '0':
	case ')':
		return kVK_ANSI_0;
	case '1':
	case '!':
		return kVK_ANSI_1;
	case '2':
	case '@':
		return kVK_ANSI_2;
	case '3':
	case '#':
		return kVK_ANSI_3;
	case '4':
	case '$':
		return kVK_ANSI_4;
	case '5':
	case '%':
		return kVK_ANSI_5;
	case '6':
	case '^':
		return kVK_ANSI_6;
	case '7':
	case '&':
		return kVK_ANSI_7;
	case '8':
	case '*':
		return kVK_ANSI_8;
	case '9':
	case '(':
		return kVK_ANSI_9;
	case 'q':
	case 'Q':
		return kVK_ANSI_Q;
	case 'w':
	case 'W':
		return kVK_ANSI_W;
	case 'e':
	case 'E':
		return kVK_ANSI_E;
	case 'r':
	case 'R':
		return kVK_ANSI_R;
	case 't':
	case 'T':
		return kVK_ANSI_T;
	case 'y':
	case 'Y':
		return kVK_ANSI_Y;
	case 'u':
	case 'U':
		return kVK_ANSI_U;
	case 'i':
	case 'I':
		return kVK_ANSI_I;
	case 'o':
	case 'O':
		return kVK_ANSI_O;
	case 'p':
	case 'P':
		return kVK_ANSI_P;
	case 'a':
	case 'A':
		return kVK_ANSI_A;
	case 's':
	case 'S':
		return kVK_ANSI_S;
	case 'd':
	case 'D':
		return kVK_ANSI_D;
	case 'f':
	case 'F':
		return kVK_ANSI_F;
	case 'g':
	case 'G':
		return kVK_ANSI_G;
	case 'h':
	case 'H':
		return kVK_ANSI_H;
	case 'j':
	case 'J':
		return kVK_ANSI_J;
	case 'k':
	case 'K':
		return kVK_ANSI_K;
	case 'l':
	case 'L':
		return kVK_ANSI_L;
	case 'z':
	case 'Z':
		return kVK_ANSI_Z;
	case 'x':
	case 'X':
		return kVK_ANSI_X;
	case 'c':
	case 'C':
		return kVK_ANSI_C;
	case 'v':
	case 'V':
		return kVK_ANSI_V;
	case 'b':
	case 'B':
		return kVK_ANSI_B;
	case 'n':
	case 'N':
		return kVK_ANSI_N;
	case 'M':
	case 'm':
		return kVK_ANSI_M;
	case ';':
	case ':':
		return kVK_ANSI_Semicolon;
	case '\'':
	case '\"':
		return kVK_ANSI_Quote;
	case ',':
	case '<':
		return kVK_ANSI_Comma; //,
	case '.':
	case '>':
		return kVK_ANSI_Period;
	case '/':
	case '?':
		return kVK_ANSI_Backslash;
	case '\\':
	case '|':
		return kVK_ANSI_Slash;
	case '{':
	case '[':
		return kVK_ANSI_LeftBracket;
	case '}':
	case ']':
		return kVK_ANSI_RightBracket;
	case '-':
	case '_':
		return kVK_ANSI_Minus;
	case '=':
	case '+':
		return kVK_ANSI_Equal;
	default:
	return kVK_Escape;
	}

}



bool getFlags(char k)
{
	switch(k)
	{
		case ')':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '(':
		case 'Q':
		case 'W':
		case 'E':
		case 'R':
		case 'T':
		case 'Y':
		case 'U':
		case 'I':
		case 'O':
		case 'P':
		case 'A':
		case 'S':
		case 'D':
		case 'F':
		case 'G':
		case 'H':
		case 'J':
		case 'K':
		case 'L':
		case 'Z':
		case 'X':
		case 'C':
		case 'V':
		case 'B':
		case 'N':
		case 'M':
		case ':':
		case '"':
		case '<':
		case '>':
		case '?':
		case '{':
		case '}':
		case '_':
		case '+':
		case '|':
			return true;
	}
	return false;

}
