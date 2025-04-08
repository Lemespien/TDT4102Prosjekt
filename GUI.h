#pragma once
#include <iostream>
#include "AnimationWindow.h"
#include "widgets/TextInput.h"
#include "widgets/Button.h"

#include "Stopwatch.h"
#include "Particle.h"
#include "SimulationController.h"

class SimulationWindow : public TDT4102::AnimationWindow
{
    private:

        // Konstanter for grafikkvinduet
        static constexpr int button_width = 80;
        static constexpr int button_height = 40;
        static constexpr int width = 1600;
        static constexpr int height = 900;
        
        bool simulation_running = false;
        bool inputHeld = false;

        Stopwatch sw;
        std::string configPath = "orbital.lemesave";
        // Medlemsfunksjoner
        void draw_particles();
        void handle_input();
        
    public:
        // konstruktør
        SimulationWindow(int x, int y, const std::string& title);
        SimulationController sc;
        // kjører programmet
        void run(std::string& pre_config);

};