#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H
//------------------------------------------------------------------------
//
//  Name:   ConsoleUtils.h
//
//  Desc:   Just a few handy utilities for dealing with consoles
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <mutex>


//default text colors can be found in wincon.h
inline void SetTextColor(WORD colors)
{
  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
  
  SetConsoleTextAttribute(hConsole, colors);
}

inline void PressAnyKeyToContinue()
{
  //change text color to white
  SetTextColor(FOREGROUND_BLUE| FOREGROUND_RED | FOREGROUND_GREEN);

  std::cout << "\n\nPress any key to continue" << std::endl; 

  while (!_kbhit()){}

  return;
}

class ConsoleUtils {

public:

    // Get the instance
    static ConsoleUtils& getInstance() {
        static ConsoleUtils instance;
        return instance;
    }

    // Print a message in the console
    // Handle the color
    // Use a mutex for multiThreading
    void PrintMessageInConsole( std::string msg, WORD color ) {

        lock.lock();
        SetTextColor( color );
        std::cout << msg << std::endl;
        lock.unlock();
    }

private:
    std::mutex lock;
    ConsoleUtils() {}
    
};



#endif