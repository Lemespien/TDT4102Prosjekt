#pragma once
#include <iostream>
#include "AnimationWindow.h"
#include "widgets/TextInput.h"
#include "widgets/Button.h"

#include "Stopwatch.h"
#include "ColorsVector.h"
#include "SimulationController.h"

class SimulationWindow : public TDT4102::AnimationWindow
{
    private:

        // Konstanter for grafikkvinduet
        static constexpr int button_width = 80;
        static constexpr int button_height = 40;
        static constexpr int width = 1600;
        static constexpr int height = 900;
        static constexpr double smoothing = 0.85;
        bool simulation_running = false;
        bool inputHeld = false;
        int drawInterval = 1;
        int drawIntCounter = 0;
        
        bool showDebug = false;
        bool paintMode = false;

        Stopwatch sw;
        std::string configPath = "orbital.lemesave";
        // Medlemsfunksjoner
        void draw_particles();
        void handle_input();
        void toggleDebug();
    public:
        // konstruktør
        SimulationWindow(int x, int y, const std::string& title);
        SimulationController sc;
        // kjører programmet
        int runCount = 0;
        double avgFPS = 0;

        void run(std::string& pre_config);

};