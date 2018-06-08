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


void key(char ch)
{
	CGKeyCode k = QtKeyCode2MacKeyCode(ch);
	CGEventRef mkey = CGEventCreateKeyboardEvent(NULL, k, true);
	//CGEventSetFlags(mkey, kCGEventFlagMaskShift);//set shift key down for above event
	CGEventPost(kCGHIDEventTap, mkey);
	CFRelease(mkey);
}

CGKeyCode QtKeyCode2MacKeyCode(char k)
{
	switch(k){
	case '0':
		return kVK_ANSI_0;
	case '1':
		return kVK_ANSI_1;
	case '2':
		return kVK_ANSI_2;
	case '3':
		return kVK_ANSI_3;
	case '4':
		return kVK_ANSI_4;
	case '5':
		return kVK_ANSI_5;
	case '6':
		return kVK_ANSI_6;
	case '7':
		return kVK_ANSI_7;
	case '8':
		return kVK_ANSI_8;
	case '9':
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
	case Qt::Key_F1:
		return kVK_F1;
	case Qt::Key_F2:
		return kVK_F2;
	case Qt::Key_F3:
		return kVK_F3;
	case Qt::Key_F4:
		return kVK_F4;
	case Qt::Key_F5:
		return kVK_F5;
	case Qt::Key_F6:
		return kVK_F6;
	case Qt::Key_F7:
		return kVK_F7;
	case Qt::Key_F8:
		return kVK_F8;
	case Qt::Key_F9:
		return kVK_F9;
	case Qt::Key_F10:
		return kVK_F10;
	case Qt::Key_F11:
		return kVK_F11;
	case Qt::Key_F12:
		return kVK_F12;
	case Qt::Key_Left:
		return kVK_LeftArrow;
	case Qt::Key_Right:
		return kVK_RightArrow;
	case Qt::Key_Down:
		return kVK_DownArrow;
	case Qt::Key_Up:
		return kVK_UpArrow;
	case Qt::Key_Return:
		return kVK_Return;
	case Qt::Key_Tab:
		return kVK_Tab;
	case Qt::Key_Space:
		return kVK_Space;
	case Qt::Key_Delete:
		return kVK_Delete;
	case Qt::Key_Escape:
		return kVK_Escape;
	case Qt::Key_Shift:
		return kVK_Shift;
	case Qt::Key_Control:
		return kVK_Control;
	case Qt::Key_CapsLock:
		return kVK_CapsLock;
	default:
		return kVK_Escape;
	}

}