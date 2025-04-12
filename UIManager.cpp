#include "UIManager.h"

UIManager::UIManager()
{

}

TDT4102::Button& UIManager::createButton(const std::string& text, std::function<void ()> callback){
    std::shared_ptr<TDT4102::Button> newButton = std::make_shared<TDT4102::Button>(TDT4102::Point(buttonPosition.x + buttonWidth*2 + (buttonWidth + buttonMarginX)*UIElements.size(), buttonPosition.y), buttonWidth, buttonHeight, text);
    // std::shared_ptr<TDT4102::Button> newButton = std::make_shared<TDT4102::Button>(TDT4102::Point(100, 100 + (buttonHeight + buttonMarginY)*UIElements.size()), buttonWidth, buttonHeight, text);
    UIElements.push_back(newButton);
    newButton->setCallback(callback);
    return *newButton;
}

std::shared_ptr<TDT4102::DropdownList> UIManager::createLoadingMenu() {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator("./savefiles")) {
        files.push_back(entry.path().string());
    }
    std::shared_ptr<TDT4102::DropdownList> loadingDropDown = std::make_shared<TDT4102::DropdownList>(TDT4102::Point(buttonPosition.x + (buttonWidth + buttonMarginX)*UIElements.size(), buttonPosition.y), buttonWidth*3, buttonHeight, files);
    UIElements.push_back(loadingDropDown);
    return loadingDropDown;
}

void UIManager::toggleUI() {
    isUiVisible = !isUiVisible;
    for (auto elementVariant : UIElements) {
        if (std::holds_alternative<std::shared_ptr<TDT4102::Button>>(elementVariant)) {
            auto element_ptr = std::get<std::shared_ptr<TDT4102::Button>>(elementVariant);
            element_ptr->setVisible(isUiVisible);
        }
        if (std::holds_alternative<std::shared_ptr<TDT4102::DropdownList>>(elementVariant)) {
            auto element_ptr = std::get<std::shared_ptr<TDT4102::DropdownList>>(elementVariant);
            element_ptr->setVisible(isUiVisible);
        }
    }
}
