#include "GUI.h"

SimulationWindow::SimulationWindow(int x, int y, const std::string& title) : 
    AnimationWindow{x, y, width, height, title},
    sc{width, height}

{
}


void SimulationWindow::run(std::string& configPath) {
    while(!should_close()) {
        if(!simulation_running) {
            if (!sw.isRunning) {
                sw.start();
            }
            try{
                // Load funksjonalitet
                sc.reset(); // Remove all the old stuff
                sc.load(configPath);
                this->configPath = configPath;
                sc.randomSpawn();
                // std::cout << "Load not implemented" << std::endl;
            }
            catch(const std::ios_base::failure& e) {
                std::cerr << "Error loading config: " << e.what() << std::endl;
                sc.loadDefault();
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
            // Doesnt change much atm. Time is better spent optimizing physics calculations.
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
        if (!sc.isPaused) { // Only track avg fps when sim is running.
            // 'Smoothing filter', learned from AIST1001
            avgFPS = (avgFPS * smoothing) + 1/tickrate * (1.0 - smoothing);
            runCount++;
        }
        // std::cout << "Run loop | avgFPS: " << avgFPS << " | Timestep: " << tickrate << " | run count: " << runCount << std::endl;
    }
}

void SimulationWindow::draw_particles() {
    std::size_t count = 0;
    for (auto& particle : sc.getParticles()) {
        draw_circle(particle->getIntPosition(), particle->radius, colorsVector.at(count));
        count = ++count % colorsVector.size();
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