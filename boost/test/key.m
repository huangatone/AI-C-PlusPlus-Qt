#include <ApplicationServices/ApplicationServices.h>
#import <Foundation/Foundation.h>



void Press(int key);
void Release(int key);
void Click(int key);


int shiftKeyCode = 56;
bool shiftIsDown = false;

void postKeyboardEvent( int keyCode )
{
    bool keyUp = false;
   

    if( keyCode == shiftKeyCode ){

        if( keyUp ){

            shiftIsDown = false;

        }else{

            shiftIsDown = true;

        }

    }

    CGEventSourceRef source = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
    CGEventRef keyEvent = CGEventCreateKeyboardEvent( source, (CGKeyCode) keyCode, !keyUp );

    if( shiftIsDown ){

        // Use Shift flag
        CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) | kCGEventFlagMaskShift );

    }else{

        // Use all existing flag except Shift
        CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) & ~kCGEventFlagMaskShift );

    }

    CGEventPost( kCGHIDEventTap, keyEvent );

    CFRelease( keyEvent );
    CFRelease( source );

}


int main(int argc, char *argv[]) {

NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSUserDefaults *args = [NSUserDefaults standardUserDefaults];

    int x = [args integerForKey:@"x"];

    usleep(60000);
    //Press(56);
    Click(x);
   // Release(56);

    //postKeyboardEvent(x);
}

void Press(int key) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, true);

    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(60);
}

void Release(int key) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key release event
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, false);

    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(60);
}

void Click(int key) {
    Press(key);
    Release(key);
}