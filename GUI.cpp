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
{}

void SimulationWindow::run(std::string& configPath) {
    while(!should_close()) {
        sw.start();
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
        draw_particles();
        handle_input();
        next_frame();
        double timebetween = sw.stop();
        // std::cout << "Framerate: " << 1/timebetween << std::endl;
    }
}

void SimulationWindow::draw_particles() {
    sc.step();
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
            // if (sc.isPaused) {
            //     draw_text(TDT4102::Point(width/2, height*3/4), "Paused");
            // }
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