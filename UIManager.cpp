#include "UIManager.h"

UIManager::UIManager()
{

}

TDT4102::Button& UIManager::createButton(const std::string& text, std::function<void ()> callback){
    
    std::shared_ptr<TDT4102::Button> newButton = std::make_shared<TDT4102::Button>(TDT4102::Point(100, 100 + (buttonHeight + buttonMarginY)*UIElements.size()), buttonWidth, buttonHeight, text);
    UIElements.push_back(newButton);
    newButton->setCallback(callback);
    return *newButton;
}

std::shared_ptr<TDT4102::DropdownList> UIManager::createLoadingMenu() {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator("./savefiles")) {
        files.push_back(entry.path().string());
    }
    std::shared_ptr<TDT4102::DropdownList> loadingDropDown = std::make_shared<TDT4102::DropdownList>(TDT4102::Point(100, 100 + (buttonHeight + buttonMarginY)*UIElements.size()), buttonWidth*3, buttonHeight, files);
    UIElements.push_back(loadingDropDown);
    return loadingDropDown;
}
