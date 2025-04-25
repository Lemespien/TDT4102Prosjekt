#pragma once
#include <iostream>
#include "AnimationWindow.h"
#include "UIManager.h"
#include "Stopwatch.h"
#include "ColorsVector.h"
#include "SimulationController.h"
#include <algorithm>

struct MouseCreatedParticle {
    Vector2 start;
    Vector2 end;
    Vector2 velVecEndPos = Vector2();
    double radius = 5;
    double mass = 10;
    bool inProgress = false;
};

class SimulationWindow : public TDT4102::AnimationWindow
{
    private:
        UIManager UI = UIManager();
        // Konstanter for grafikkvinduet
        static constexpr int button_width = 80;
        static constexpr int button_height = 40;
        static constexpr int defaultWidth = 1600;
        static constexpr int defaultHeight = 900;
        static constexpr int uiNotificationWidth = 300;

        static constexpr double smoothing = 0.85;

        bool simulation_running = false;
        bool inputHeld = false;
        bool isLeftMouseStarted = false;
        MouseCreatedParticle mCP;
        TDT4102::Point mouseStartPos;
        TDT4102::Point mouseEndPos;
        int drawInterval = 1;
        int drawIntCounter = 0;
    
        bool showDebug = false;
        bool paintMode = false;

        Stopwatch stopwatch;
        std::string configPath = "./savefiles/orbital.lemesave";
        // Medlemsfunksjoner
        void draw_particles();
        void draw_debug(TDT4102::Point pCenter, int const& radius, Vector2 const& acc, Vector2 const& vel, int const& count = 0);
        void draw_ui_notifications();
        void handle_input();
        void toggleDebug();
        void resetButtonClicked();
        void exitButtonClicked();
        void handleLoadDropdown(const std::string& filepath);
    public:
    // konstruktør
        SimulationWindow(int x, int y, const std::string& title);
        SimulationController sc;
        // kjører programmet
        int runCount = 0;
        double avgFPS = 0;
        const std::string& getConfigPath();

        void createUI();
        void run(std::string& pre_config);

};