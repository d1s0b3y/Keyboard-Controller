// gcc -lXInput -o Controller_To_Keyboard.exe Controller_to_Keyboard.c
#include <stdio.h>   // Printing and input
#include <stdbool.h> // Enables booleans
#include <string.h>  // Strings
#include <math.h>    // Math
#include <stdlib.h>  // Random
#include <time.h>    // Time
#include <ctype.h>   // formating stuff???
#include <windows.h> // Windows
#include <Xinput.h>  // Xbox controller

typedef struct // make this structure so I can return multiple values becasue C does not allow multiple values to be returned
{
    unsigned char A, B, X, Y, LT, LS, RT, RS, Start, Back, LeftThumbStick, RightThumbStick, DPadUp, DPadDown, DPadLeft, DPadRight;
    short leftStickX, leftStickY, rightStickX, rightStickY;
} ControllerInput;
double reducer_mouse = 0;
double reducer_scroll = 0;

// ------------------------------
// --------------------------------------------------------
// ------------------------------
// MISC
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
bool is_key_pressed(WORD key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

// ------------------------------
// --------------------------------------------------------
// ------------------------------
// MOUSE
void click_mouse_down(char button)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    WORD buttonToPress;
    if (button == 'l')
    {
        buttonToPress = VK_LBUTTON;
    }
    else if (button == 'r')
    {
        buttonToPress = VK_RBUTTON;
    }
    else if (button == 'm')
    {
        buttonToPress = VK_MBUTTON;
    }
    if (!(is_key_pressed(buttonToPress)))
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
    }
}
void click_mouse_up(char button)
{

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    WORD buttonToPress;
    if (button == 'l')
    {
        buttonToPress = VK_LBUTTON;
    }
    else if (button == 'r')
    {
        buttonToPress = VK_RBUTTON;
    }
    else if (button == 'm')
    {
        buttonToPress = VK_MBUTTON;
    }
    if ((is_key_pressed(buttonToPress)))
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
    }
}

void scroll_mouse(char dir, int amount)
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
} // move it BY x and y not to x and y

// ------------------------------
// KEYBOARD
void press_key_down(WORD key)
{
    INPUT keyboardInput = {0}; // had to put this outside the switch because it didn't work otherwise for some reason
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
// ACTION SENDER
void action_down(WORD key, int contVal)
{

    switch (key)
    {
    case 108: // ASCII code for 'l

        click_mouse_down('l');
        break;

    case 114: // ASCII code for 'r'

        click_mouse_down('r');
        break;

    case 109: // ASCII code for 'm'

        click_mouse_down('m');
        break;

    case 100: // ASCII code for 'd'

        scroll_mouse('d', abs(contVal));
        break;

    case 117: // ASCII code for 'u'

        scroll_mouse('u', abs(contVal));
        break;

    case 88: // ASCII for 'X' (+ direction)

        move_mouse(contVal, 0, reducer_mouse);
        break;

    case 120: // ASCII for 'x' (- direction)
        move_mouse(-contVal, 0, reducer_mouse);
        break;

    case 89: // ASCII for 'Y' (+ direction)

        move_mouse(0, contVal, reducer_mouse);
        break;

    case 121: // ASCII for 'y' (- direction)

        move_mouse(0, -contVal, reducer_mouse);
        break;

    default:

        press_key_down(key);
        break;
    }
}
void action_up(WORD key)
{
    int contVal = 0;
    switch (key)
    {
    case 108: // ASCII code for 'l

        click_mouse_up('l');
        break;

    case 114: // ASCII code for 'r'

        click_mouse_up('r');
        break;

    case 109: // ASCII code for 'm'

        click_mouse_up('m');
        break;

    case 100: // ASCII code for 'd'

        scroll_mouse('d', abs(contVal));
        break;

    case 117: // ASCII code for 'u'

        scroll_mouse('u', abs(contVal));
        break;

    case 88: // ASCII for 'X' (+ direction)

        move_mouse(contVal, 0, reducer_mouse);
        break;

    case 120: // ASCII for 'x' (- direction)
        move_mouse(-contVal, 0, reducer_mouse);
        break;

    case 89: // ASCII for 'Y' (+ direction)

        move_mouse(0, contVal, reducer_mouse);
        break;

    case 121: // ASCII for 'y' (- direction)

        move_mouse(0, -contVal, reducer_mouse);
        break;

    default:

        press_key_up(key);
        break;
    }
}

// ------------------------------
// STOP ALL ACTIONS
void stop_all()
{
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP means key release

    // Iterate through all possible virtual key codes
    for (int key = 0x01; key <= 0xFE; key++)
    {
        // Sleep(1);
        input.ki.wVk = key;
        SendInput(1, &input, sizeof(INPUT));
    }
    // Sleep(1);
    click_mouse_up('l');
    // Sleep(1);
    click_mouse_up('m');
    // Sleep(1);
    click_mouse_up('r');
    // Sleep(1);
    scroll_mouse('u', 0);
    // Sleep(1);
    scroll_mouse('d', 0);
}

// ------------------------------
// --------------------------------------------------------
// ------------------------------
// CONTROLLER
bool connect_controller(bool printOutputCon, bool printOutputNotCon)
{
    XINPUT_STATE state;

    if (XInputGetState(0, &state) != ERROR_SUCCESS)
    {
        if (printOutputNotCon == true)
        {
            printf("Controller not connected\n");
        }
        return false;
    }
    else
    {
        if (printOutputCon == true)
        {
            printf("Controller connected\n");
        }
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
// --------------------------------------------------------
// ------------------------------
// GUI
WORD translateGUI(const char *GUIin)
{

    const WORD buttonCorrelationKeyboardSide[101] = {0xC0, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, VK_OEM_MINUS, VK_OEM_PLUS, 0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, VK_OEM_4, VK_OEM_6, VK_OEM_5, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4A, 0x4B, 0x4C, VK_OEM_1, VK_OEM_7, 0x5A, 0x58, 0x43, 0x56, 0x42, 0x4E, 0x4D, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, VK_F13, VK_F14, VK_F15, VK_F16, VK_F17, VK_F18, VK_F19, VK_F20, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_NUMPAD0, VK_RWIN, VK_LWIN, VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_NUMLOCK, VK_LSHIFT, VK_RSHIFT, VK_LMENU, VK_RMENU, VK_RCONTROL, VK_LCONTROL, VK_BACK, VK_DELETE, VK_TAB, VK_CAPITAL, VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN};
    const char GUIButtonsTranslation[101][6] = {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "NPad1", "NPad2", "NPad3", "NPad4", "NPad5", "NPad6", "NPad7", "NPad8", "NPad9", "NPad0", "RWin", "LWin", "PgUp", "PgDn", "End", "Home", "NLock", "LShft", "RShft", "LAlt", "RAlt", "RCtrl", "LCtrl", "Bcksp", "Del", "Tab", "CapLk", "Space", "LArow", "RArow", "UArow", "DArow"};
    // "Fn" (keys not availible for use rn ;_; )
    for (int i = 0; i < (sizeof GUIButtonsTranslation / sizeof GUIButtonsTranslation[0]); i++)
    {
        if (strcasecmp(GUIin, "Lclck") == 0)
        {
            return 'l';
            break;
        }
        else if (strcasecmp(GUIin, "Rclck") == 0)
        {
            return 'r';
            break;
        }
        else if (strcasecmp(GUIin, "Mclck") == 0)
        {
            return 'm';
            break;
        }
        else if (strcasecmp(GUIin, "ScrlD") == 0)
        {
            return 'd';
            break;
        }
        else if (strcasecmp(GUIin, "ScrlU") == 0)
        {
            return 'u';
            break;
        }
        else if (strcasecmp(GUIin, "MovX+") == 0)
        {
            return 'X';
            break;
        }
        else if (strcasecmp(GUIin, "MovX-") == 0)
        {
            return 'x';
            break;
        }
        else if (strcasecmp(GUIin, "MovY+") == 0)
        {
            return 'Y';
            break;
        }
        else if (strcasecmp(GUIin, "MovY-") == 0)
        {
            return 'y';
            break;
        }
        else if (strcasecmp(GUIin, GUIButtonsTranslation[i]) == 0)
        {
            return buttonCorrelationKeyboardSide[i];
            break;
        }
    }
}

// ------------------------------
// --------------------------------------------------------
// ------------------------------

int main()
{

    printf("Code running\n\n-----DO NOT CLOSE THIS APPLICATION-----\n\n");

    XInputEnable(true);

    // Variables
    reducer_mouse = 0.001;
    reducer_scroll = 0.1;
    // bool connected = false;

    // translating stuff from GUI into actual
    int buttonCorrelationDelay[24] = {0, 0, 120, 120, 0, 0, 120, 120, 0, 0, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    WORD buttonCorrelationCodeKey[24];
    char FromGUIstringTranslation[24][6] = {"Space", "LShft", "F", "E", "Lclck", "Rclck", "Esc", "L", "LCtrl", "Mclck", "F5", "Q", "F2", "T", "ScrlU", "ScrlD", "D", "A", "W", "S", "MovX+", "MovX-", "MovY-", "MovY+"};
    // char FromGUIstringTranslation[24][6] = {A, B, X, Y, RS, LS, Start, Back, LeftThumbStick, RightThumbStick, DPadUp, DPadDown, DPadLeft, DPadRight, LT, RT, leftStickX, -leftStickX, leftStickY, -leftStickY, rightStickX, -rightStickX, rightStickY, -rightStickY};
    WORD MetaButtonsCodeKey[2];
    char FromGUIMetaButtonsStr[2][6] = {"LWin", "CapLk"};
    // char FromGUIMetaButtonsStr[2][6] = {Exit, Mode};

    for (int i = 0; i < (sizeof(FromGUIstringTranslation) / sizeof(FromGUIstringTranslation[0])); i++)
    {
        buttonCorrelationCodeKey[i] = translateGUI(FromGUIstringTranslation[i]);
        // printf("%hu\n", translateGUI(FromGUIstringTranslation[i]));
    }

    for (int i = 0; i < (sizeof(FromGUIMetaButtonsStr) / sizeof(FromGUIMetaButtonsStr[0])); i++)
    {
        MetaButtonsCodeKey[i] = translateGUI(FromGUIMetaButtonsStr[i]);
        // printf("%hu\n", translateGUI(FromGUIstringTranslation[i]));
    }
    // printf("successully registered the keys\n");

    do // inital connect
    {
        (void)0;
    } while (!connect_controller(true, true));

    // printf("entering loop\n");
    while (!is_key_pressed(MetaButtonsCodeKey[0])) // while the off key is not pressed
    {

        do // connect controller everytime (in case it gets disconnected midgame)
        {
            (void)0;
        } while (!connect_controller(false, true));

        ControllerInput input = get_controller_input();
        int ControllerInputArr[24] = {input.A, input.B, input.X, input.Y, input.RS, input.LS, input.Start, input.Back, input.LeftThumbStick, input.RightThumbStick, input.DPadUp, input.DPadDown, input.DPadLeft, input.DPadRight, input.LT, input.RT, input.leftStickX, -input.leftStickX, input.leftStickY, -input.leftStickY, input.rightStickX, -input.rightStickX, input.rightStickY, -input.rightStickY};

        if (!toggle_state(is_key_pressed(MetaButtonsCodeKey[1])))
        {
            for (int i = 0; i < (sizeof ControllerInputArr / sizeof ControllerInputArr[0]); i++)
            {
                // printf("\n%d", ControllerInputArr[i]);

                if (ControllerInputArr[i] > 0)
                {
                    action_down(buttonCorrelationCodeKey[i], ControllerInputArr[i]);
                    Sleep(buttonCorrelationDelay[i]);
                }
                else if (!ControllerInputArr[i])
                {
                    action_up(buttonCorrelationCodeKey[i]);
                }
            }
        }
        // printf("\nout of for loop");
        Sleep(1); // Required or else minecraft doesnt work for some reason (tick rate?)
    }

    printf("\nOut of loop, ending keys. DO NOT CLOSE THIS APPLICATION\nThe application will close itself when done, do not be alarmed if it closes randomly");
    if (is_key_pressed(MetaButtonsCodeKey[0]))
    {
        stop_all();
    }
    printf("\n\nCode completed succesfully");
    return 0;
}

// DEGUGGING/TESTING CODE ------------------
/* presses down keys using if statments instead of for loop
// replacment for the for loop that sends the keys in the main function
    if (input.A)
    {
        // printf("A Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[0]);
        Sleep(buttonCorrelationDelay[0]);
    }
    else if (!input.A && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[0]);
    }

    if (input.B)
    {
        // printf("B Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[1]);
        Sleep(buttonCorrelationDelay[1]);
    }
    else if (!input.B && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[1]);
    }

    if (input.X)
    {
        // printf("X Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[2]);
        Sleep(buttonCorrelationDelay[2]);
    }
    else if (!input.X && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[2]);
    }

    if (input.Y)
    {
        // printf("Y Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[3]);
        Sleep(buttonCorrelationDelay[3]);
    }
    else if (!input.Y && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[3]);
    }

    if (input.LT > 0)
    {
        // printf("Left Trigger pressed%d\n", input.LT);
        press_key_down(buttonCorrelationCodeKey[4]);
        Sleep(buttonCorrelationDelay[4]);
    }
    else if (input.LT <= 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {
        press_key_up(buttonCorrelationCodeKey[4]);
    }

    if (input.RT > 0)
    {
        // printf("Right Trigger pressed%d\n", input.RT);
        press_key_down(buttonCorrelationCodeKey[5]);
        Sleep(buttonCorrelationDelay[5]);
    }
    else if (input.RT <= 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {
        press_key_up(buttonCorrelationCodeKey[5]);
    }

    if (input.RS)
    {
        // printf("Right Shoulder pressed\n");
        press_key_down(buttonCorrelationCodeKey[6]);
        Sleep(buttonCorrelationDelay[6]);
    }
    else if (!input.RS && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {
        press_key_up(buttonCorrelationCodeKey[6]);
    }

    if (input.LS)
    {

        // printf("Left Shoulder pressed\n");
        press_key_down(buttonCorrelationCodeKey[7]);
        Sleep(buttonCorrelationDelay[7]);
    }
    else if (!input.LS && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[7]);
    }

    if (input.Start)
    {
        // printf("Start Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[8]);
        Sleep(buttonCorrelationDelay[8]);
    }
    else if (!input.Start && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[8]);
    }

    if (input.Back)
    {
        // printf("Back Button pressed\n");

        press_key_down(buttonCorrelationCodeKey[9]);
        Sleep(buttonCorrelationDelay[9]);
    }
    else if (!input.Back && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[9]);
    }

    if (input.LeftThumbStick)
    {

        // printf("Left Thumbstick pressed\n");

        press_key_down(buttonCorrelationCodeKey[10]);
        Sleep(buttonCorrelationDelay[10]);
    }
    else if (!input.LeftThumbStick && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[10]);
    }

    if (input.RightThumbStick)
    {
        // printf("Right Thumbstick pressed\n");
        press_key_down(buttonCorrelationCodeKey[11]);
        Sleep(buttonCorrelationDelay[11]);
    }
    else if (!input.RightThumbStick && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {
        press_key_up(buttonCorrelationCodeKey[11]);
    }

    if (input.DPadUp)
    {
        // printf("DPad Up pressed\n");

        press_key_down(buttonCorrelationCodeKey[12]);
        Sleep(buttonCorrelationDelay[12]);
    }
    else if (!input.DPadUp && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[12]);
    }

    if (input.DPadDown)
    {
        // printf("DPad Down pressed\n");

        press_key_down(buttonCorrelationCodeKey[13]);
        Sleep(buttonCorrelationDelay[13]);
    }
    else if (!input.DPadDown && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[13]);
    }

    if (input.DPadLeft)
    {
        // printf("DPad Left pressed\n");

        press_key_down(buttonCorrelationCodeKey[14]);
        Sleep(buttonCorrelationDelay[14]);
    }
    else if (!input.DPadLeft && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[14]);
    }

    if (input.DPadRight)
    {

        // printf("DPad Right pressed\n");

        press_key_down(buttonCorrelationCodeKey[15]);
        Sleep(buttonCorrelationDelay[15]);
    }
    else if (!input.DPadRight && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[15]);
    }

    if (input.leftStickX > 0)
    {
        // printf("Left Stick X = %d\n", input.leftStickX);

        press_key_down(buttonCorrelationCodeKey[16]);
        Sleep(buttonCorrelationDelay[16]);
    }
    else if (input.leftStickX == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[16]);
    }
    if (input.leftStickX < 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {
        // printf("Left Stick X = %d\n", input.leftStickX);

        press_key_down(buttonCorrelationCodeKey[17]);
        Sleep(buttonCorrelationDelay[17]);
    }
    else if (input.leftStickX == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[17]);
    }

    if (input.leftStickY > 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_down(buttonCorrelationCodeKey[18]);
        // printf("Left Stick Y = %d\n", input.leftStickY);
        Sleep(buttonCorrelationDelay[18]);
    }
    else if (input.leftStickY == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[18]);
    }

    if (input.leftStickY < 0)
    {

        press_key_down(buttonCorrelationCodeKey[19]);
        // printf("Left Stick Y = %d\n", input.leftStickY);
        Sleep(buttonCorrelationDelay[19]);
    }
    else if (input.leftStickY == 0 && !toggle_state(is_key_pressed(VK_CAPITAL)))
    {

        press_key_up(buttonCorrelationCodeKey[19]);
    }

    move_mouse(input.rightStickX, -input.rightStickY, reducer);
*/
/* Tells you if a key is pressed
// TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING

Sleep(1000);
while (!is_key_pressed(VK_LWIN))
{
    printf("%d\n", is_key_pressed(VK_LBUTTON));
}

// TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING TESTING
*/
/* arrays with controller inputs seperated as continous and binary
// use to replace the other array thats an input for the for loop and has all of these same values
unsigned char ControllerInArrBin[] = {input.A, input.B, input.X, input.Y, input.RS, input.LS, input.Start, input.Back, input.LeftThumbStick, input.RightThumbStick, input.DPadUp, input.DPadDown, input.DPadLeft, input.DPadRight};
int ControllerInArrCont[] = {input.LT, input.RT, input.leftStickX, input.leftStickY, input.rightStickX, input.rightStickY};
*/
// DEGUGGING/TESTING CODE ------------------

/* Keyboard Mappings
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
VK_OEM_CLEAR 	0xFE
*/
