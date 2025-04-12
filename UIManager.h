#pragma once
#include "widgets/TextInput.h"
#include "widgets/Button.h"
#include "widgets/DropdownList.h"
#include <functional>
#include <memory>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <variant>


using UIElementVariant = std::variant<std::shared_ptr<TDT4102::Button>, std::shared_ptr<TDT4102::DropdownList>>;
class UIManager {
    private:
        const TDT4102::Point buttonPosition {0, 0};
        const unsigned int buttonWidth = 100;
        const unsigned int buttonHeight = 40;
        static constexpr unsigned int buttonMarginY = 20;
        static constexpr unsigned int buttonMarginX = 10;
        const std::string buttonLabel = "Reset";
        bool isUiVisible = true;
    public:

        UIManager();
        TDT4102::Button& createButton(const std::string& text, std::function<void ()> callback);
        std::shared_ptr<TDT4102::DropdownList> createLoadingMenu();
        std::vector<UIElementVariant> UIElements;
        void toggleUI();
};