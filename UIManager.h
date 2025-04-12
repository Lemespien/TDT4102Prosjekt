#pragma once
#include "widgets/TextInput.h"
#include "widgets/Button.h"
#include <functional>
#include <memory>
class UIManager {
    private:
        const TDT4102::Point buttonPosition {100, 100};
        const unsigned int buttonWidth = 100;
        const unsigned int buttonHeight = 40;
        static constexpr unsigned int buttonMarginY = 20;
        const std::string buttonLabel = "Reset";

    public:
        UIManager();
        TDT4102::Button& createButton(const std::string& text, std::function<void ()> callback);
        std::vector<std::shared_ptr<TDT4102::Button>> buttons;
        TDT4102::Button button {buttonPosition, buttonWidth, buttonHeight, buttonLabel};
};