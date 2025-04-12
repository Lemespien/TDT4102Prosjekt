//
// This is example code from Chapter 2.2 "The classic first program" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
// This program outputs the message "Hello, World!" to the monitor

#include "std_lib_facilities.h"
#include "GUI.h"
#include "Test_Vector2.h"
#include "UIManager.h"
//------------------------------------------------------------------------------'

// C++ programs start by executing the function main
int main() {
    // Show a nice message in the Terminal window
    cout << "Hello, World!" << endl;
    // Vector2Test();
    Stopwatch stopwatch;
    TDT4102::Point start_point{50, 50};
    SimulationWindow sw{start_point.x, start_point.y, "Simulation"};
    // UI.create_UI();
    std::string configPath = "save1.lemesave";
    stopwatch.start();
    // const TDT4102::Point buttonPosition {100, 100};
    // const unsigned int buttonWidth = 100;
    // const unsigned int buttonHeight = 40;
    // const std::string buttonLabel = "Click me!";
    // TDT4102::Button button {buttonPosition, buttonWidth, buttonHeight, buttonLabel};
    // sw.add(button);
    sw.createUI();
    sw.run(configPath);
    
    double timeElapsed = stopwatch.stop();
    double averageFPS = sw.runCount / timeElapsed;
    std::cout << "Run time: " << timeElapsed << " | avgFPS: " << averageFPS << std::endl;

    // This lets the operating system (Windows, Mac, Linux, etc.) know that the program
    // did not encounter any errors
    return 0;
}

//------------------------------------------------------------------------------
