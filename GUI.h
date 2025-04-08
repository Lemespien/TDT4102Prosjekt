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
        
        bool simulation_running = false;
        bool inputHeld = false;

        int width = 1920;
        int height = 1080;

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