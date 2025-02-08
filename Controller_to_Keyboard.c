// gcc -lXInput -o test.exe Controller_to_Keyboard.c
#include <stdio.h>   // Printing and input
#include <stdbool.h> // Enables booleans
#include <string.h>  // Strings
#include <math.h>    // Math
#include <stdlib.h>  // Random
#include <time.h>    // Time
#include <ctype.h>   // formating stuff???
#include <windows.h> // Windows
#include <Xinput.h>  // Xbox controller

/*
CONTROL SCHEME:
left stick - WASD
right stick - Mouse movment
Left sholder - right click
right sholder - left click
left trigger - scroll up
right trigger scroll down
A - spacebar
B - shift
X - F
Y - E
Start - esc
Back - l
left thumb stick press - control
right thumb stick press - middle click
Dpad up - F5
Dpad down - Q
Dpad left - F2
Dpad right - T
Windows key - end program
Capslock - (disables the controller*, when testing i discovered that when the program is active, if you try to move your character in minecraft with WASD it dowsnt work, so if you turn on capslock it toggles it to keyboard mode where that works but the controller doesnt work (if somethign weird is going on try pressing capslock 2x))
CURRENTLY THIS ONLY SUPPORTS xBOX CONTROLLERS ON WINDOWS
*/
typedef struct // make this structure so I can return multiple values becasue C does not allow multiple values to be returned
{
    unsigned char A, B, X, Y, LT, LS, RT, RS, Start, Back, LeftThumbStick, RightThumbStick, DPadUp, DPadDown, DPadLeft, DPadRight;
    short leftStickX, leftStickY, rightStickX, rightStickY;
} ControllerInput;

bool toggle_state(bool condition)
{
    static bool state = false;         // Stores the toggled state
    static bool prevCondition = false; // Stores the previous condition state

    if (condition && !prevCondition)
    {
        state = !state; // Toggle the state
    }

    prevCondition = condition; // Update previous state for next call

    return state;
}
// ------------------------------

bool is_key_pressed(WORD key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
void press_key_down(WORD key)
{

    INPUT keyboardInput = {0};
    keyboardInput.type = INPUT_KEYBOARD;
    keyboardInput.ki.wVk = key;
    keyboardInput.ki.dwFlags = 0; // 0 means key press
    SendInput(1, &keyboardInput, sizeof(INPUT));
}
void press_key_up(WORD key)
{

    INPUT keyboardInput = {0};
    keyboardInput.type = INPUT_KEYBOARD;
    keyboardInput.ki.wVk = key;
    keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP means key release
    SendInput(1, &keyboardInput, sizeof(INPUT));
}

// ------------------------------

void click_mouse_down(char button, bool press, bool *buttonState)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    if (press && !*buttonState)
    {
        if (button == 'l')
        {
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        }
        else if (button == 'r')
        {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        }
        else if (button == 'm')
        {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        }
        SendInput(1, &input, sizeof(INPUT));
        *buttonState = true;
    }
}
void click_mouse_up(char button, bool press, bool *buttonState)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    if (!press && *buttonState)
    {
        if (button == 'l')
        {
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        }
        else if (button == 'r')
        {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        }
        else if (button == 'm')
        {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        }
        SendInput(1, &input, sizeof(INPUT));
        *buttonState = false;
    }
}

void scroll_mouse(char dir, unsigned char amount)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    if (dir == 'u')
    {
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = amount;
    }
    else if (dir == 'd')
    {
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = -amount;
    }
    SendInput(1, &input, sizeof(INPUT));
}
void move_mouse(int x, int y, double multiplier)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = x * multiplier;
    input.mi.dy = y * multiplier;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
} // move it ***by*** x and y not to x and y

// ------------------------------

bool connect_controller()
{
    XINPUT_STATE state;

    if (XInputGetState(0, &state) != ERROR_SUCCESS)
    {
        printf("Controller not connected\n");
        return false;
    }
    else
    {
        printf("Controller connected\n");
        return true;
    }
} // connect to the controller
ControllerInput get_controller_input()
{
    XINPUT_STATE state;

    ControllerInput input = {0}; // Initialize all values to 0

    if (XInputGetState(0, &state) == ERROR_SUCCESS)
    {
        WORD buttons = state.Gamepad.wButtons; // Get the bitmask for pressed buttons
        const int DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

        input.leftStickX = state.Gamepad.sThumbLX;
        input.leftStickY = state.Gamepad.sThumbLY;
        input.rightStickX = state.Gamepad.sThumbRX;
        input.rightStickY = state.Gamepad.sThumbRY;

        if (abs(input.leftStickX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            input.leftStickX = state.Gamepad.sThumbLX;
            // printf("Left Stick X = %d \n", input.leftStickX);
        }
        else
        {
            input.leftStickX = 0;
        }
        if (abs(input.leftStickY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            input.leftStickY = state.Gamepad.sThumbLY;
            // printf("Left Stick Y = %d \n", input.leftStickY);
        }
        else
        {
            input.leftStickY = 0;
        }
        if (abs(input.rightStickX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            input.rightStickX = state.Gamepad.sThumbRX;
            // printf("Right Stick X = %d \n", input.rightStickX);
        }
        else
        {
            input.rightStickX = 0;
        }
        if (abs(input.rightStickY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            input.rightStickY = state.Gamepad.sThumbRY;
            // printf("Right Stick Y = %d \n", input.rightStickY);
        }
        else
        {
            input.rightStickY = 0;
        }

        if (buttons & XINPUT_GAMEPAD_A)
        {
            input.A = 1;
            // printf("A button pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_B)
        {
            input.B = 1;
            // printf("B button pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_X)
        {
            input.X = 1;
            // printf("X button pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_Y)
        {
            input.Y = 1;
            // printf("Y button pressed\n");
        }
        if (state.Gamepad.bLeftTrigger > 0)
        {
            input.LT = state.Gamepad.bLeftTrigger;
            // printf("Left Trigger pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
        {
            input.LS = 1;
            // printf("Left Shoulder (LB) pressed\n");
        }
        if (state.Gamepad.bRightTrigger > 0)
        {
            input.RT = state.Gamepad.bRightTrigger;
            // printf("Right Trigger pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
        {
            input.RS = 1;
            // printf("Right Shoulder (RB) pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_START)
        {
            input.Start = 1;
            // printf("Start button pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_BACK)
        {
            input.Back = 1;
            // printf("Back button pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_LEFT_THUMB)
        {
            input.LeftThumbStick = 1;
            // printf("Left Thumbstick pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
        {
            input.RightThumbStick = 1;
            // printf("Right Thumbstick pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_DPAD_UP)
        {
            input.DPadUp = 1;
            // printf("DPad Up pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
        {
            input.DPadDown = 1;
            // printf("DPad Down pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)
        {
            input.DPadLeft = 1;
            // printf("DPad Left pressed\n");
        }
        if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
        {
            input.DPadRight = 1;
            // printf("DPad Right pressed\n");
        }
    }

    return input;
}

// ------------------------------
void release_all_keys()
{

    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP means key release

    // Iterate through all possible virtual key codes
    for (int key = 0x01; key <= 0xFE; key++)
    {
        Sleep(1);
        input.ki.wVk = key;
        SendInput(1, &input, sizeof(INPUT));
    }
}

void stop_all()
{
    bool release = true;
    release_all_keys();
    Sleep(1);
    click_mouse_up('l', false, &release);
    Sleep(1);
    click_mouse_up('m', false, &release);
    Sleep(1);
    click_mouse_up('r', false, &release);
    Sleep(1);
    scroll_mouse('u', 0);
    Sleep(1);
    scroll_mouse('d', 0);
}

int main()
{

    printf("Code running\n\n");

    XInputEnable(true);

    double reducer = 0.0005;
    double reducer_scroll = 0.1;
    bool connected = false;
    bool leftMouseButtonState = false;
    bool rightMouseButtonState = false;
    bool middleMouseButtonState = false;

    while (!is_key_pressed(VK_LWIN)) // while the windows key is not pressed
    {
        do // connect controller everytime (incase it gets disconnected midgame)
        {
            connected = connect_controller();
        } while (connected == false);

        ControllerInput input = get_controller_input();

        if (input.A)
        {
            // printf("A Button pressed\n");

            press_key_down(VK_SPACE);
        }
        else if (!input.A && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_SPACE);
        }

        if (input.B)
        {
            // printf("B Button pressed\n");

            press_key_down(VK_SHIFT);
            Sleep(10);
        }
        else if (!input.B && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_SHIFT);
        }

        if (input.X)
        {
            // printf("X Button pressed\n");

            press_key_down(0x46);
            Sleep(120);
        }
        else if (!input.X && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x46);
        }

        if (input.Y)
        {
            // printf("Y Button pressed\n");

            press_key_down(0x45);
            Sleep(90);
        }
        else if (!input.Y && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x45);
        }

        if (input.LT > 0)
        {
            // printf("Left Trigger pressed%d\n", input.LT);
            scroll_mouse('u', input.LT * reducer_scroll);
        }
        else if (input.LT <= 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
        }

        if (input.RT > 0)
        {
            // printf("Right Trigger pressed%d\n", input.RT);
            scroll_mouse('d', input.RT * reducer_scroll);
        }
        else if (input.RT <= 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
        }

        if (input.RS)
        {
            printf("Right Shoulder pressed\n");
            click_mouse_down('l', true, &leftMouseButtonState);
            leftMouseButtonState = true;
        }
        else if (!input.RS && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
            click_mouse_up('l', false, &leftMouseButtonState);
            leftMouseButtonState = false;
        }
        if (input.LS)
        {

            printf("Left Shoulder pressed\n");
            click_mouse_down('r', true, &rightMouseButtonState);
            rightMouseButtonState = true;
        }
        else if (!input.LS && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            click_mouse_up('r', false, &rightMouseButtonState);
            rightMouseButtonState = false;
        }

        if (input.Start && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
            // printf("Start Button pressed\n");

            press_key_down(VK_ESCAPE);
            Sleep(120);
        }
        else if (!input.Start && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_ESCAPE);
        }

        if (input.Back)
        {
            // printf("Back Button pressed\n");

            press_key_down(0x4C);
            Sleep(120);
        }
        else if (!input.Back && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x4C);
        }

        if (input.LeftThumbStick)
        {

            // printf("Left Thumbstick pressed\n");

            press_key_down(VK_LCONTROL);
        }
        else if (!input.LeftThumbStick && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_LCONTROL);
        }

        if (input.RightThumbStick)
        {
            // printf("Right Thumbstick pressed\n");
            click_mouse_down('m', input.RightThumbStick, &middleMouseButtonState);
            middleMouseButtonState = true;
        }
        else if (!input.RightThumbStick && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
            click_mouse_up('m', input.RightThumbStick, &middleMouseButtonState);
            middleMouseButtonState = false;
        }

        if (input.DPadUp)
        {
            // printf("DPad Up pressed\n");

            press_key_down(VK_F5);
            Sleep(90);
        }
        else if (!input.DPadUp && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_F5);
        }
        if (input.DPadDown)
        {
            // printf("DPad Down pressed\n");

            press_key_down(0x51);
            Sleep(90);
        }
        else if (!input.DPadDown && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x51);
        }
        if (input.DPadLeft)
        {
            // printf("DPad Left pressed\n");

            press_key_down(VK_F2);
            Sleep(90);
        }
        else if (!input.DPadLeft && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(VK_F2);
        }
        if (input.DPadRight)
        {

            // printf("DPad Right pressed\n");

            press_key_down(0x54);
            Sleep(90);
        }
        else if (!input.DPadRight && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x54);
        }

        if (input.leftStickX > 0)
        {
            // printf("Left Stick X = %d\n", input.leftStickX);

            press_key_down(0x44);
        }
        else if (input.leftStickX == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x44);
        }
        if (input.leftStickX < 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {
            // printf("Left Stick X = %d\n", input.leftStickX);

            press_key_down(0x41);
        }
        else if (input.leftStickX == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x41);
        }
        if (input.leftStickY > 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_down(0x57);
            // printf("Left Stick Y = %d\n", input.leftStickY);
        }
        else if (input.leftStickY == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x57);
        }
        if (input.leftStickY < 0)
        {

            press_key_down(0x53);
            // printf("Left Stick Y = %d\n", input.leftStickY);
        }
        else if (input.leftStickY == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
        {

            press_key_up(0x53);
        }

        move_mouse(input.rightStickX, -input.rightStickY, reducer);
        Sleep(1);
    }
    printf("\nOut of loop, ending keys");
    if (is_key_pressed(VK_LWIN))
    {
        stop_all();
    }
    printf("\n\nCode completed succesfully");
    return 0;
}

/*
VK_LBUTTON 	0x01 	Left mouse button
VK_RBUTTON 	0x02 	Right mouse button
VK_CANCEL 	0x03 	Control-break processing
VK_MBUTTON 	0x04 	Middle mouse button
VK_XBUTTON1 	0x05 	X1 mouse button
VK_XBUTTON2 	0x06 	X2 mouse button
- 	0x07 	Reserved
VK_BACK 	0x08 	BACKSPACE key
VK_TAB 	0x09 	TAB key
- 	0x0A-0B 	Reserved
VK_CLEAR 	0x0C 	CLEAR key
VK_RETURN 	0x0D 	ENTER key
- 	0x0E-0F 	Unassigned
VK_SHIFT 	0x10 	SHIFT key
VK_CONTROL 	0x11 	CTRL key
VK_MENU 	0x12 	ALT key
VK_PAUSE 	0x13 	PAUSE key
VK_CAPITAL 	0x14 	CAPS LOCK key
VK_KANA 	0x15 	IME Kana mode
VK_HANGUL 	0x15 	IME Hangul mode
VK_IME_ON 	0x16 	IME On
VK_JUNJA 	0x17 	IME Junja mode
VK_FINAL 	0x18 	IME final mode
VK_HANJA 	0x19 	IME Hanja mode
VK_KANJI 	0x19 	IME Kanji mode
VK_IME_OFF 	0x1A 	IME Off
VK_ESCAPE 	0x1B 	ESC key
VK_CONVERT 	0x1C 	IME convert
VK_NONCONVERT 	0x1D 	IME nonconvert
VK_ACCEPT 	0x1E 	IME accept
VK_MODECHANGE 	0x1F 	IME mode change request
VK_SPACE 	0x20 	SPACEBAR
VK_PRIOR 	0x21 	PAGE UP key
VK_NEXT 	0x22 	PAGE DOWN key
VK_END 	0x23 	END key
VK_HOME 	0x24 	HOME key
VK_LEFT 	0x25 	LEFT ARROW key
VK_UP 	0x26 	UP ARROW key
VK_RIGHT 	0x27 	RIGHT ARROW key
VK_DOWN 	0x28 	DOWN ARROW key
VK_SELECT 	0x29 	SELECT key
VK_PRINT 	0x2A 	PRINT key
VK_EXECUTE 	0x2B 	EXECUTE key
VK_SNAPSHOT 	0x2C 	PRINT SCREEN key
VK_INSERT 	0x2D 	INS key
VK_DELETE 	0x2E 	DEL key
VK_HELP 	0x2F 	HELP key
    0x30 	0 key
    0x31 	1 key
    0x32 	2 key
    0x33 	3 key
    0x34 	4 key
    0x35 	5 key
    0x36 	6 key
    0x37 	7 key
    0x38 	8 key
    0x39 	9 key
- 	0x3A-40 	Undefined
    0x41 	A key
    0x42 	B key
    0x43 	C key
    0x44 	D key
    0x45 	E key
    0x46 	F key
    0x47 	G key
    0x48 	H key
    0x49 	I key
    0x4A 	J key
    0x4B 	K key
    0x4C 	L key
    0x4D 	M key
    0x4E 	N key
    0x4F 	O key
    0x50 	P key
    0x51 	Q key
    0x52 	R key
    0x53 	S key
    0x54 	T key
    0x55 	U key
    0x56 	V key
    0x57 	W key
    0x58 	X key
    0x59 	Y key
    0x5A 	Z key
VK_LWIN 	0x5B 	Left Windows key
VK_RWIN 	0x5C 	Right Windows key
VK_APPS 	0x5D 	Applications key
- 	0x5E 	Reserved
VK_SLEEP 	0x5F 	Computer Sleep key
VK_NUMPAD0 	0x60 	Numeric keypad 0 key
VK_NUMPAD1 	0x61 	Numeric keypad 1 key
VK_NUMPAD2 	0x62 	Numeric keypad 2 key
VK_NUMPAD3 	0x63 	Numeric keypad 3 key
VK_NUMPAD4 	0x64 	Numeric keypad 4 key
VK_NUMPAD5 	0x65 	Numeric keypad 5 key
VK_NUMPAD6 	0x66 	Numeric keypad 6 key
VK_NUMPAD7 	0x67 	Numeric keypad 7 key
VK_NUMPAD8 	0x68 	Numeric keypad 8 key
VK_NUMPAD9 	0x69 	Numeric keypad 9 key
VK_MULTIPLY 	0x6A 	Multiply key
VK_ADD 	0x6B 	Add key
VK_SEPARATOR 	0x6C 	Separator key
VK_SUBTRACT 	0x6D 	Subtract key
VK_DECIMAL 	0x6E 	Decimal key
VK_DIVIDE 	0x6F 	Divide key
VK_F1 	0x70 	F1 key
VK_F2 	0x71 	F2 key
VK_F3 	0x72 	F3 key
VK_F4 	0x73 	F4 key
VK_F5 	0x74 	F5 key
VK_F6 	0x75 	F6 key
VK_F7 	0x76 	F7 key
VK_F8 	0x77 	F8 key
VK_F9 	0x78 	F9 key
VK_F10 	0x79 	F10 key
VK_F11 	0x7A 	F11 key
VK_F12 	0x7B 	F12 key
VK_F13 	0x7C 	F13 key
VK_F14 	0x7D 	F14 key
VK_F15 	0x7E 	F15 key
VK_F16 	0x7F 	F16 key
VK_F17 	0x80 	F17 key
VK_F18 	0x81 	F18 key
VK_F19 	0x82 	F19 key
VK_F20 	0x83 	F20 key
VK_F21 	0x84 	F21 key
VK_F22 	0x85 	F22 key
VK_F23 	0x86 	F23 key
VK_F24 	0x87 	F24 key
- 	0x88-8F 	Reserved
VK_NUMLOCK 	0x90 	NUM LOCK key
VK_SCROLL 	0x91 	SCROLL LOCK key
- 	0x92-96 	OEM specific
- 	0x97-9F 	Unassigned
VK_LSHIFT 	0xA0 	Left SHIFT key
VK_RSHIFT 	0xA1 	Right SHIFT key
VK_LCONTROL 	0xA2 	Left CONTROL key
VK_RCONTROL 	0xA3 	Right CONTROL key
VK_LMENU 	0xA4 	Left ALT key
VK_RMENU 	0xA5 	Right ALT key
VK_BROWSER_BACK 	0xA6 	Browser Back key
VK_BROWSER_FORWARD 	0xA7 	Browser Forward key
VK_BROWSER_REFRESH 	0xA8 	Browser Refresh key
VK_BROWSER_STOP 	0xA9 	Browser Stop key
VK_BROWSER_SEARCH 	0xAA 	Browser Search key
VK_BROWSER_FAVORITES 	0xAB 	Browser Favorites key
VK_BROWSER_HOME 	0xAC 	Browser Start and Home key
VK_VOLUME_MUTE 	0xAD 	Volume Mute key
VK_VOLUME_DOWN 	0xAE 	Volume Down key
VK_VOLUME_UP 	0xAF 	Volume Up key
VK_MEDIA_NEXT_TRACK 	0xB0 	Next Track key
VK_MEDIA_PREV_TRACK 	0xB1 	Previous Track key
VK_MEDIA_STOP 	0xB2 	Stop Media key
VK_MEDIA_PLAY_PAUSE 	0xB3 	Play/Pause Media key
VK_LAUNCH_MAIL 	0xB4 	Start Mail key
VK_LAUNCH_MEDIA_SELECT 	0xB5 	Select Media key
VK_LAUNCH_APP1 	0xB6 	Start Application 1 key
VK_LAUNCH_APP2 	0xB7 	Start Application 2 key
- 	0xB8-B9 	Reserved
VK_OEM_1 	0xBA 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
VK_OEM_PLUS 	0xBB 	For any country/region, the + key
VK_OEM_COMMA 	0xBC 	For any country/region, the , key
VK_OEM_MINUS 	0xBD 	For any country/region, the - key
VK_OEM_PERIOD 	0xBE 	For any country/region, the . key
VK_OEM_2 	0xBF 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
VK_OEM_3 	0xC0 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key
- 	0xC1-DA 	Reserved
VK_OEM_4 	0xDB 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
VK_OEM_5 	0xDC 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
VK_OEM_6 	0xDD 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
VK_OEM_7 	0xDE 	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
VK_OEM_8 	0xDF 	Used for miscellaneous characters; it can vary by keyboard.
- 	0xE0 	Reserved
- 	0xE1 	OEM specific
VK_OEM_102 	0xE2 	The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard
- 	0xE3-E4 	OEM specific
VK_PROCESSKEY 	0xE5 	IME PROCESS key
- 	0xE6 	OEM specific
VK_PACKET 	0xE7 	Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
- 	0xE8 	Unassigned
- 	0xE9-F5 	OEM specific
VK_ATTN 	0xF6 	Attn key
VK_CRSEL 	0xF7 	CrSel key
VK_EXSEL 	0xF8 	ExSel key
VK_EREOF 	0xF9 	Erase EOF key
VK_PLAY 	0xFA 	Play key
VK_ZOOM 	0xFB 	Zoom key
VK_NONAME 	0xFC 	Reserved
VK_PA1 	0xFD 	PA1 key
VK_OEM_CLEAR 	0xFE 	Clear key
*/
