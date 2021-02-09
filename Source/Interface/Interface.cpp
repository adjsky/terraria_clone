//
// Created by adjsky on 15.01.2021.
//

#include <SFML/Window/Mouse.hpp>

#include "Interface.h"
#include "../Core/Engine.h"
#include "../Util/Serialization/GameSerialization.h"

Interface::Interface(sf::RenderWindow &window) :
    gui_{ window },
    window_{ window }
{
    Engine::getResourceManager()->initializeFonts();
    constructGameInterface();
    constructMainMenu();
    constructPlayMenu();
    constructConsole();
    constructMenu();
}

void Interface::handleEvent(sf::Event& e) {
    gui_.handleEvent(e);
}

void Interface::draw() {
    gui_.draw();
}

bool Interface::gameInterfaceIsShown() const {
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    return gameInterface->isVisible();
}

bool Interface::inventoryIsOpen() const {
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    if (gameInterface->isVisible()) {
        tgui::Group::Ptr inventory{ gameInterface->get<tgui::Group>("inventory") };
        return inventory->isVisible();
    }
    return false;
}

bool Interface::consoleIsShown() const {
    auto console{ gui_.get("console") };
    return console->isVisible();
}

bool Interface::hotBarIsShown() const{
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    if (gameInterface->isVisible()) {
        tgui::Group::Ptr hotBar{ gameInterface->get<tgui::Group>("hotBar") };
        return hotBar->isVisible();
    }
    return false;
}

bool Interface::gameMenuIsOpen() const {
    auto menu{ gui_.get("menu") };
    return menu->isVisible();
}

void Interface::showGameInterface(bool condition) {
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    gameInterface->setVisible(condition);
}

void Interface::showInventory(bool condition) {
    tgui::Group::Ptr inventory{ gui_.get<tgui::Group>("inventory") };
    inventory->setVisible(condition);
}

void Interface::showHotBar(bool condition) {
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    hotBar->setVisible(condition);
}

void Interface::showConsole(bool condition) {
    tgui::Group::Ptr console{ gui_.get<tgui::Group>("console") };
    console->setVisible(condition);
    if (condition) {
        tgui::EditBox::Ptr consoleInput{ console->get<tgui::EditBox>("input") };
        consoleInput->setFocused(true);

        // clear previously entered text
        consoleInput->setText("");
    }
}

void Interface::showGameMenu(bool condition) {
    tgui::Group::Ptr menu{ gui_.get<tgui::Group>("menu") };
    menu->setVisible(condition);
}

void Interface::showMainMenu(bool condition) {
    tgui::Group::Ptr menu{ gui_.get<tgui::Group>("mainMenu") };
    menu->setVisible(condition);
}

void Interface::updateHealth(const Player &player) {
    auto* resourceManager{ Engine::getResourceManager() };
    auto healthBar{ gui_.get<tgui::Group>("healthBar") };
    int playerHealth{ player.getHealth() };
    healthBar->removeAllWidgets();
    int fullHearts{ playerHealth / 10 };
    int halfHearts{ (playerHealth - fullHearts * 10) / 5 };
    int emptyHearts{ (100 - playerHealth) / 10 };
    for (int i = 0; i < fullHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({resourceManager->getTexture(ResourceManager::HEALTH), {0, 0, 22, 20} }) };
        heart->setPosition({"parent.parent.hotBar.picture.position + 35 + (22.0f + 8.0f) * "  + std::to_string(i)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i));
    }
    for (int i = 0; i < halfHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({resourceManager->getTexture(ResourceManager::HEALTH), {22, 0, 22, 20} }) };
        heart->setPosition({"parent.heart" + std::to_string(fullHearts-1) + ".position + (22.0f + 8.0f) * "  + std::to_string(i+1)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i+fullHearts));
    }
    for (int i = 0; i < emptyHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({resourceManager->getTexture(ResourceManager::HEALTH), {44, 0, 22, 20} }) };
        heart->setPosition({"parent.heart" + std::to_string(fullHearts+halfHearts-1) + ".position + (22.0f + 8.0f) * "  + std::to_string(i+1)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i+fullHearts+halfHearts));
    }
}

void Interface::updateHotBar(const Player& player) {
    auto* resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    tgui::Group::Ptr hotBarItems{ hotBar->get<tgui::Group>("items") };
    hotBarItems->removeAllWidgets();
    const auto& playerHotBar{ player.getHotBar() };
    for (int x = 0; x < playerHotBar.getSize().x; x++) {
        const auto& cell{ playerHotBar.getCell(x, 0) };
        if (cell.blockType != BlockType::AIR) {
            sf::IntRect blockRect{ BlockDatabase::getData(cell.blockType).textureRect };
            tgui::Picture::Ptr blockPicture{ tgui::Picture::create({ resourceManager->getTexture(ResourceManager::BLOCKS),
                                                                         { static_cast<unsigned int>(blockRect.left), static_cast<unsigned int>(blockRect.top), static_cast<unsigned int>(blockRect.width), static_cast<unsigned int>(blockRect.height) }}) };
            blockPicture->setOrigin(0.5f, 0.5f);
            blockPicture->setPosition({"parent.parent.hotBar.cells.cell" + std::to_string(x) + ".position + parent.parent.hotBar.cells.cell" +  std::to_string(x) + ".size / 2"});
            blockPicture->setSize({"parent.parent.hotBar.cells.cell" + std::to_string(x) + ".size * 0.4"});
            blockPicture->ignoreMouseEvents(true);
            hotBarItems->add(blockPicture);

            if (cell.amount != 1) {
                tgui::Label::Ptr label{ tgui::Label::create(std::to_string(cell.amount)) };
                label->setInheritedFont(resourceManager->getFont(ResourceManager::YUSEI));
                label->getRenderer()->setTextColor({110,55,0});
                label->getRenderer()->setTextOutlineColor(sf::Color::Black);
                label->getRenderer()->setTextOutlineThickness(0.5f);
                label->setTextSize(13);
                label->setPosition({"parent.parent.hotBar.cells.cell" + std::to_string(x) + ".left + 5"},
                                   {"parent.parent.hotBar.cells.cell" + std::to_string(x) + ".top + 20"});
                label->ignoreMouseEvents(true);
                hotBarItems->add(label);
            }
        }
    }
}

void Interface::updateInventory(const Player& player) {
    auto* resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    tgui::Group::Ptr inventory{ gameInterface->get<tgui::Group>("inventory") };
    tgui::Group::Ptr inventoryItems{ inventory->get<tgui::Group>("items") };
    inventoryItems->removeAllWidgets();
    const auto& playerBackpack{ player.getBackpack() };
    for (int x = 0; x < playerBackpack.getSize().x; x++) {
        for (int y = 0; y < playerBackpack.getSize().y; y++) {
            const auto& cell{ playerBackpack.getCell(x, y) };
            if (cell.blockType != BlockType::AIR) {
                std::stringstream cellName{  };
                sf::IntRect blockRect{ BlockDatabase::getData(cell.blockType).textureRect };
                tgui::Picture::Ptr blockPicture{ tgui::Picture::create({ resourceManager->getTexture(ResourceManager::BLOCKS),
                                                                         { static_cast<unsigned int>(blockRect.left), static_cast<unsigned int>(blockRect.top), static_cast<unsigned int>(blockRect.width), static_cast<unsigned int>(blockRect.height) }}) };
                blockPicture->setOrigin(0.5f, 0.5f);
                blockPicture->setPosition({"parent.parent.inventory.cells.cell" + std::to_string(x) + std::to_string(y) + ".position + parent.parent.inventory.cells.cell" + std::to_string(x) + std::to_string(y) + ".size / 2"});
                blockPicture->setSize({"parent.parent.inventory.cells.cell" + std::to_string(x) + std::to_string(y) + ".size * 0.5"});
                blockPicture->ignoreMouseEvents(true);
                inventoryItems->add(blockPicture);

                if (cell.amount != 1) {
                    tgui::Label::Ptr label{ tgui::Label::create(std::to_string(cell.amount)) };
                    label->setInheritedFont(resourceManager->getFont(ResourceManager::YUSEI));
                    label->getRenderer()->setTextColor({110,55,0});
                    label->getRenderer()->setTextOutlineColor(sf::Color::Black);
                    label->getRenderer()->setTextOutlineThickness(0.5f);
                    label->setTextSize(13);
                    label->setPosition({"parent.parent.inventory.cells.cell" + std::to_string(x) + std::to_string(y) + ".left + 3"},
                                       {"parent.parent.inventory.cells.cell" + std::to_string(x) + std::to_string(y) + ".top + 13"});
                    label->ignoreMouseEvents(true);
                    inventoryItems->add(label);
                }
            }
        }
    }
}

void Interface::updateAttachedItem(const Player &player, bool swapped) {
    auto* resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    tgui::Picture::Ptr attachedItem{ gameInterface->get<tgui::Picture>("attachedItem") };
    if (player.hasAttachedItem) {
        if (attachedItem && !swapped) {
            attachedItem->setPosition(sf::Mouse::getPosition(window_).x, sf::Mouse::getPosition(window_).y);
        }
        else {
            sf::IntRect blockRect{ BlockDatabase::getData(player.attachedItem.blockType).textureRect };
            if (swapped) gameInterface->remove(attachedItem);
            attachedItem = tgui::Picture::create({ resourceManager->getTexture(ResourceManager::BLOCKS),
                                                   { static_cast<unsigned int>(blockRect.left), static_cast<unsigned int>(blockRect.top), static_cast<unsigned int>(blockRect.width), static_cast<unsigned int>(blockRect.height) }});
            attachedItem->setScale(0.5f);
            attachedItem->ignoreMouseEvents(true);
            gameInterface->add(attachedItem, "attachedItem");
        }
    }
    else {
        gameInterface->remove(attachedItem);
    }
}

void Interface::highlightHotBarCell(const Player& player) {
    auto* resourceManager{ Engine::getResourceManager() };
    static int previousHighlightedCell{ 0 };
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    hotBar->get<tgui::Picture>("cell"+std::to_string(previousHighlightedCell))->getRenderer()->setTexture({ resourceManager->getTexture(ResourceManager::INVENTORY_CELL),{ 0, 0, 31, 31 }});
    hotBar->get<tgui::Picture>("cell"+std::to_string(player.getHotBarIndex()))->getRenderer()->setTexture({ resourceManager->getTexture(ResourceManager::INVENTORY_CELL), {31, 0, 31, 31 }});
    previousHighlightedCell = player.getHotBarIndex();
}

void Interface::constructMainMenu() {
    auto resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr mainMenu{ tgui::Group::create() };

    tgui::Picture::Ptr playButton{ tgui::Picture::create() };
    tgui::Texture playTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                                                                 { 0, 0, 549, 95 } };
    playButton->getRenderer()->setTexture(playTexture);
    playButton->setSize(playButton->getSize() * 0.6f);
    playButton->setPosition({ "parent.size / 2" },
                            { "parent.size / 2 - size / 2 - 10" });
    playButton->setOrigin(0.5f, 0.5f);
    playButton->onClick([this](){
        bool hasSavedGame{ GameSerialization::isGameSaved() };
        if (hasSavedGame) {
            auto mainMenu{ gui_.get<tgui::Group>("mainMenu") };
            auto playMenu{ gui_.get<tgui::Group>("playMenu") };
            mainMenu->setVisible(false);
            playMenu->setVisible(true);
        }
        else {
            Engine::getEventSystem()->trigger<GameEvent::NewWorldButtonClicked>();
        }
    });
    mainMenu->add(playButton, "playButton");

    tgui::Picture::Ptr exitButton{ tgui::Picture::create() };
    tgui::Texture exitTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                               { 0, 95, 549, 95 } };
    exitButton->getRenderer()->setTexture(exitTexture);
    exitButton->setSize(exitButton->getSize() * 0.6f);
    exitButton->onClick([](){
        Engine::getEventSystem()->trigger<GameEvent::ExitButtonClicked>();
    });
    exitButton->setPosition({ "parent.size / 2" },
                            { "parent.size / 2 + size / 2 + 10" });
    exitButton->setOrigin(0.5f, 0.5f);
    mainMenu->add(exitButton, "exitButton");

    gui_.add(mainMenu, "mainMenu");
}

void Interface::constructPlayMenu() {
    auto resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr playMenu{ tgui::Group::create() };
    playMenu->setVisible(false);

    tgui::Picture::Ptr continueButton{ tgui::Picture::create() };
    tgui::Texture continueTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                                   { 0, 190, 549, 95 } };
    continueButton->getRenderer()->setTexture(continueTexture);
    continueButton->setSize(continueButton->getSize() * 0.6f);
    continueButton->setPosition({ "parent.size / 2" },
                                { "parent.size / 2 - size / 2 - 10" });
    continueButton->setOrigin(0.5f, 0.5f);
    continueButton->onClick([this, playMenu](){
        Engine::getEventSystem()->trigger<GameEvent::ContinueMenuButtonClicked>();
        showGameInterface(true);
        playMenu->setVisible(false);
    });
    playMenu->add(continueButton, "continueButton");

    tgui::Picture::Ptr newWorldButton{ tgui::Picture::create() };
    tgui::Texture newWorldTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                                   { 0, 285, 549, 95 } };
    newWorldButton->getRenderer()->setTexture(newWorldTexture);
    newWorldButton->setSize(newWorldButton->getSize() * 0.6f);
    newWorldButton->setPosition({ "parent.size / 2" },
                                { "parent.size / 2 + size / 2 + 10" });
    newWorldButton->setOrigin(0.5f, 0.5f);
    newWorldButton->onClick([this, playMenu](){
        Engine::getEventSystem()->trigger<GameEvent::NewWorldButtonClicked>();
        showGameInterface(true);
        playMenu->setVisible(false);
    });
    playMenu->add(newWorldButton, "newWorldButton");

    gui_.add(playMenu, "playMenu");
}

void Interface::constructGameInterface() {
    tgui::Group::Ptr gameInterface{ tgui::Group::create() };
    gameInterface->setVisible(false);
    gui_.add(gameInterface, "gameInterface");
    constructHotBar();
    constructHealthBar();
    constructInventory();
}

void Interface::constructHotBar() {
    auto* resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    // general group to hold all hot bar related widgets
    tgui::Group::Ptr hotBar{ tgui::Group::create() };

    tgui::Picture::Ptr hotBarPicture{ tgui::Picture::create(resourceManager->getTexture(ResourceManager::HOTBAR)) };
    hotBarPicture->setSize(hotBarPicture->getSize() * 1 / 1.5f);
    hotBarPicture->setPosition("parent.width / 2 - width / 2", "parent.height - height - 3");
    hotBar->add(hotBarPicture, "picture");

    tgui::Group::Ptr hotBarCells{ tgui::Group::create() };
    hotBar->add(hotBarCells, "cells");

    for (int x = 0; x < PLAYER_HOTBAR_SIZE; x++) {
        tgui::Picture::Ptr cell{ tgui::Picture::create() };
        tgui::Texture texture{ resourceManager->getTexture(ResourceManager::INVENTORY_CELL),
                                                           { 0, 0, 31, 31 } };
        cell->getRenderer()->setTexture(texture);
        cell->setInheritedOpacity(0.75f);
        cell->setSize(cell->getSize() * 1.2f);
        cell->setPosition({ "parent.picture.position + 26 + (size + 4) * " + std::to_string(x) },
                          "parent.picture.position + 22");
        cell->onClick([x](){
            Engine::getEventSystem()->trigger<GameEvent::HotBarCellPressed>(x);
        });
        hotBarCells->add(cell, "cell" + std::to_string(x));
    }

    // general group to hold pictures of items visible in hot bar
    tgui::Group::Ptr items{ tgui::Group::create() };
    hotBar->add(items, "items");
    gameInterface->add(hotBar, "hotBar");
}

void Interface::constructHealthBar() {
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    tgui::Group::Ptr healthBar{ tgui::Group::create() };
    gameInterface->add(healthBar, "healthBar");
}

void Interface::constructInventory() {
    tgui::Group::Ptr gameInterface{ gui_.get<tgui::Group>("gameInterface") };
    auto* resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr inventory{ tgui::Group::create() };
    tgui::Picture::Ptr inventoryPicture{ tgui::Picture::create(resourceManager->getTexture(ResourceManager::INVENTORY)) };
    inventoryPicture->setOrigin(0.5f, 0.5f);
    inventoryPicture->setSize(inventoryPicture->getSize() * 0.8f);
    inventoryPicture->setPosition("parent.size / 2");
    inventory->add(inventoryPicture, "picture");

    tgui::Group::Ptr inventoryCells{ tgui::Group::create() };
    inventory->add(inventoryCells, "cells");
    for (int y = 0; y < PLAYER_BACKPACK_SIZE.y; y++) {
        for (int x = 0; x < PLAYER_BACKPACK_SIZE.x; x++) {
            tgui::Picture::Ptr cell{ tgui::Picture::create() };
            tgui::Texture texture{ resourceManager->getTexture(ResourceManager::INVENTORY_CELL),
                                   { 0, 0, 31, 31 } };
            cell->getRenderer()->setTexture(texture);
            cell->setPosition({ "60 + parent.parent.picture.position + (size + 2) * " + std::to_string(x)},
                              { "parent.parent.picture.position - parent.parent.picture.size / 3 + (size + 2) * " + std::to_string(y)});
            cell->onClick([x, y]() {
                Engine::getEventSystem()->trigger<GameEvent::InventoryCellPressed>(x, y);
            });
            inventoryCells->add(cell, "cell" + std::to_string(x)  + std::to_string(y));
        }
    }

    tgui::Group::Ptr items{ tgui::Group::create() };
    inventory->add(items, "items");
    inventory->setVisible(false);
    gameInterface->add(inventory, "inventory");
}

void Interface::constructConsole() {
    // general group to hold console related widgets
    tgui::Group::Ptr console{ tgui::Group::create() };

    tgui::EditBox::Ptr consoleInput{ tgui::EditBox::create() };
    consoleInput->onReturnKeyPress([consoleInput]() {
        Engine::getEventSystem()->trigger<GameEvent::ConsoleEntered>(consoleInput->getText().toStdString());
        consoleInput->setText("");
    });

    console->add(consoleInput, "input");
    console->setVisible(false);
    gui_.add(console, "console");
}

void Interface::constructMenu() {
    auto resourceManager{ Engine::getResourceManager() };
    tgui::Group::Ptr menu{ tgui::Group::create() };

    tgui::Picture::Ptr background{ tgui::Picture::create() };
    tgui::Texture backgroundTexture{ resourceManager->getTexture(ResourceManager::GAME_MENU_BACKGROUND) };
    background->getRenderer()->setTexture(backgroundTexture);
    background->getRenderer()->setOpacity(0.5f);
    background->setSize("parent.size");
    menu->add(background, "background");

    tgui::Picture::Ptr continueButton{ tgui::Picture::create() };
    tgui::Texture continueTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                               { 0, 190 , 549, 95 }};
    continueButton->getRenderer()->setTexture(continueTexture);
    continueButton->onClick([](){
        Engine::getEventSystem()->trigger<GameEvent::ContinueGameButtonClicked>();
    });
    continueButton->setSize(continueButton->getSize() * 0.5f);
    continueButton->setOrigin(0.5f, 0.5f);
    continueButton->setPosition({ "parent.size / 2" },
                                { "parent.size / 2 - size - 10" });
    menu->add(continueButton, "continueButton");

    tgui::Picture::Ptr mainMenuButton{ tgui::Picture::create() };
    tgui::Texture mainMenuTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                                   { 0, 380, 549, 95 }};
    mainMenuButton->getRenderer()->setTexture(mainMenuTexture);
    mainMenuButton->onClick([](){
        Engine::getEventSystem()->trigger<GameEvent::MainMenuButtonClicked>();
    });
    mainMenuButton->setSize(mainMenuButton->getSize() * 0.5f);
    mainMenuButton->setOrigin(0.5f, 0.5f);
    mainMenuButton->setPosition("parent.size / 2");

    menu->add(mainMenuButton, "mainMenuButton");

    tgui::Picture::Ptr exitButton{ tgui::Picture::create() };
    tgui::Texture exitTexture{ resourceManager->getTexture(ResourceManager::BUTTONS),
                               { 0, 95, 549, 95 }};
    exitButton->getRenderer()->setTexture(exitTexture);
    exitButton->onClick([](){
        Engine::getEventSystem()->trigger<GameEvent::ExitButtonClicked>();
    });
    exitButton->setSize(exitButton->getSize() * 0.5f);
    exitButton->setOrigin(0.5f, 0.5f);
    exitButton->setPosition({ "parent.size / 2" },
                            { "parent.size / 2 + size + 10" });
    menu->add(exitButton, "exitButton");

    menu->setVisible(false);
    gui_.add(menu, "menu");
}

