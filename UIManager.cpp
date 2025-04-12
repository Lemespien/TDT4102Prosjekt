#include "UIManager.h"

UIManager::UIManager()
{

}

TDT4102::Button& UIManager::createButton(const std::string& text, std::function<void ()> callback){
    
    std::shared_ptr<TDT4102::Button> newButton = std::make_shared<TDT4102::Button>(TDT4102::Point(100, 100 + (buttonHeight + buttonMarginY)*buttons.size()), buttonWidth, buttonHeight, text);
    buttons.push_back(newButton);
    newButton->setCallback(callback);
    return *newButton;
}
