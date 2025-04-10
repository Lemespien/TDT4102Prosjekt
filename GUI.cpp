#include "GUI.h"

std::vector<TDT4102::Color> colors{
    TDT4102::Color::crimson,
    TDT4102::Color::black,
    TDT4102::Color::dark_blue,
    TDT4102::Color::purple,
    TDT4102::Color::tomato,
    TDT4102::Color::royal_blue,
    TDT4102::Color::rosy_brown,
    TDT4102::Color::dark_olivegreen,
};

SimulationWindow::SimulationWindow(int x, int y, const std::string& title) : 
    AnimationWindow{x, y, width, height, title},
    sc{width, height}
{
}

int drawInterval = 1;
int drawIntCounter = 0;
double avgFPS = 0;
const double smoothing = 0.85;
void SimulationWindow::run(std::string& configPath) {
    while(!should_close()) {
        if (!sw.isRunning) {
            sw.start();
        }
            // double timestep = sw.stop();
        // std::cout << "time elapsed: " << timestep << std::endl;

        if(!simulation_running) {
            try{
                // Load funksjonalitet
                sc.reset(); // Remove all the old stuff
                sc.load(configPath);
                configPath = configPath;
                // std::cout << "Load not implemented" << std::endl;
            }
            catch(const std::ios_base::failure& e) {
                std::cerr << "Error loading config: " << e.what() << std::endl;
            }
            std::cout << "Starting simulation" << std::endl;
            simulation_running = true;
        }
        double tickrate = sw.stop();
        sw.start();
        sc.step(tickrate);
        drawIntCounter++;
        if (drawIntCounter > drawInterval) {
            // An attempt at limiting draw calls compared to "physics" calculations.
            // this is currently frame rate dependent.... which means lower FPS == choppier movement
            keep_previous_frame(false);
            next_frame();
            draw_particles();
            if (sc.isPaused) {
                // magic placement numbers that totaly work for all window sizes.... /s
                draw_rectangle(TDT4102::Point(width/2-275, height/2-5), 560, 60);
                draw_text(TDT4102::Point(width/2-265, height/2), "Paused - Press space to unpause", TDT4102::Color::black, 40U);
            }
            keep_previous_frame(true);
            drawIntCounter = 0;
        }

        handle_input();

        // next_frame();
        if (!sc.isPaused) {
            // 'Smoothing filter', learned from AIST1001
            avgFPS = (avgFPS * smoothing) + 1/tickrate * (1.0 - smoothing);
            runCount++;
        }
        // std::cout << "Run loop | avgFPS: " << avgFPS << " | Timestep: " << tickrate << " | run count: " << runCount << std::endl;
    }
}

void SimulationWindow::draw_particles() {
    int count = 0;
    for (auto& particle : sc.getParticles()) {
        draw_circle(particle->getIntPosition(), particle->radius, colors.at(count));
        count = ++count % colors.size();
    }

    return;
}

void SimulationWindow::handle_input() {
    bool current_0_state = is_key_down(KeyboardKey::SPACE); // Pause
    bool current_1_state = is_key_down(KeyboardKey::R); // Reset

    if (current_0_state) {
        if (!inputHeld) {
            sc.toggleRunState();
            inputHeld = true;
        }
    } else if (current_1_state) {
        if (!inputHeld) {
            sc.reset();
            sc.load(configPath);
        }
    } else if (inputHeld) {
        inputHeld = false;
    }


    return;
}