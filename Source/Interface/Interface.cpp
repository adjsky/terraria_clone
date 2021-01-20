//
// Created by adjsky on 15.01.2021.
//

#include <iostream>

#include "Interface.h"
#include "../ResourceManager/ResourceManager.h"

Interface::Interface(sf::RenderWindow &window) :
    gui_{ window },
    window_{ window }
{
    ResourceManager::initializeFonts();
    constructHotBar();
    constructHealthBar();
    constructInventory();
    constructConsole();
}

void Interface::handleEvent(sf::Event& e) {
    gui_.handleEvent(e);
}

void Interface::draw() {
    gui_.draw();
}

void Interface::showInventory() {
    auto inventory{ gui_.get("inventory") };
    inventory->setVisible(!inventory->isVisible());
}

void Interface::showHotBar() {
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    hotBar->setVisible(!hotBar->isVisible());
}

void Interface::showConsole() {
    tgui::Group::Ptr console{ gui_.get<tgui::Group>("console") };
    console->setVisible(!console->isVisible());
    if (console->isVisible()) {
        tgui::EditBox::Ptr consoleInput{ console->get<tgui::EditBox>("input") };
        consoleInput->setFocused(true);

        // clear previous entered text
        consoleInput->setText("");
    }
}

void Interface::updateHealth(const Player &player) {
    auto healthBar{ gui_.get<tgui::Group>("healthBar") };
    int playerHealth{ player.getHealth() };
    healthBar->removeAllWidgets();
    int fullHearts{ playerHealth / 10 };
    int halfHearts{ (playerHealth - fullHearts * 10) / 5 };
    int emptyHearts{ (100 - playerHealth) / 10 };
    for (int i = 0; i < fullHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({ResourceManager::getTexture(ResourceManager::HEALTH), {0, 0, 22, 20} }) };
        heart->setPosition({"parent.parent.hotBar.picture.position + 35 + (22.0f + 8.0f) * "  + std::to_string(i)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i));
    }
    for (int i = 0; i < halfHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({ResourceManager::getTexture(ResourceManager::HEALTH), {22, 0, 22, 20} }) };
        heart->setPosition({"parent.heart" + std::to_string(fullHearts-1) + ".position + (22.0f + 8.0f) * "  + std::to_string(i+1)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i+fullHearts));
    }
    for (int i = 0; i < emptyHearts; i++) {
        tgui::Picture::Ptr heart{ tgui::Picture::create({ResourceManager::getTexture(ResourceManager::HEALTH), {44, 0, 22, 20} }) };
        heart->setPosition({"parent.heart" + std::to_string(fullHearts+halfHearts-1) + ".position + (22.0f + 8.0f) * "  + std::to_string(i+1)},
                           "parent.parent.hotBar.picture.position - 13.0f");
        healthBar->add(heart, "heart"+std::to_string(i+fullHearts+halfHearts));
    }
}

void Interface::updateHotBar(const Player& player) {
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    tgui::Group::Ptr hotBarItems{ hotBar->get<tgui::Group>("items") };
    hotBarItems->removeAllWidgets();
    const auto& playerHotBar{player.getInventory() };
    for (int i = 0; i < 10; i++) {
        const auto& cell{ playerHotBar.getCell(i, 0) };
        if (cell.blockType != BlockType::AIR) {
            sf::IntRect blockRect{ BlockDatabase::getData(cell.blockType).textureRect };
            tgui::Picture::Ptr blockPicture{ tgui::Picture::create({ ResourceManager::getTexture(ResourceManager::BLOCK),
                                                                         { static_cast<unsigned int>(blockRect.left), static_cast<unsigned int>(blockRect.top), static_cast<unsigned int>(blockRect.width), static_cast<unsigned int>(blockRect.height) }}) };
            blockPicture->setOrigin(0.5f, 0.5f);
            blockPicture->setPosition({"parent.parent.hotBar.cell" + std::to_string(i) + ".position + parent.parent.hotBar.cell" +  std::to_string(i) + ".size / 2"});
            blockPicture->setSize({"parent.parent.hotBar.cell" + std::to_string(i) + ".size * 0.4"});
            hotBarItems->add(blockPicture);

            if (cell.amount != 1) {
                tgui::Label::Ptr label{ tgui::Label::create(std::to_string(cell.amount)) };
                label->setInheritedFont(ResourceManager::getFont(ResourceManager::YUSEI));
                label->getRenderer()->setTextColor({110,55,0});
                label->getRenderer()->setTextOutlineColor(sf::Color::Black);
                label->getRenderer()->setTextOutlineThickness(0.5f);
                label->setTextSize(13);
                label->setPosition({"parent.parent.hotBar.cell" + std::to_string(i) + ".left + 5"},
                                   {"parent.parent.hotBar.cell" + std::to_string(i) + ".top + 20"});
                hotBarItems->add(label);
            }
        }
    }
}

void Interface::highlightHotBarCell(const Player& player) {
    static int previousHighlightedCell{ 0 };
    tgui::Group::Ptr hotBar{ gui_.get<tgui::Group>("hotBar") };
    hotBar->get<tgui::Picture>("cell"+std::to_string(previousHighlightedCell))->getRenderer()->setTexture({ ResourceManager::getTexture(ResourceManager::INVENTORY_CELL),{ 0, 0, 31, 31 }});
    hotBar->get<tgui::Picture>("cell"+std::to_string(player.getHeldItem()))->getRenderer()->setTexture({ ResourceManager::getTexture(ResourceManager::INVENTORY_CELL),{ 31, 0, 31, 31 }});
    previousHighlightedCell = player.getHeldItem();
}

void Interface::constructHotBar() {
    // general group to hold all hot bar related widgets
    tgui::Group::Ptr hotBar{ tgui::Group::create() };

    tgui::Picture::Ptr hotBarPicture{ tgui::Picture::create(ResourceManager::getTexture(ResourceManager::HOTBAR)) };
    hotBarPicture->setSize(hotBarPicture->getSize() * 1 / 1.5f);
    hotBarPicture->setPosition("parent.width / 2 - width / 2", "parent.height - height - 3");
    hotBar->add(hotBarPicture, "picture");

    // add cell pictures to hot bar
    for (int i = 0; i < 10; i++) {
        tgui::Picture::Ptr cell{ tgui::Picture::create( {ResourceManager::getTexture(ResourceManager::INVENTORY_CELL),
                                                         { 0, 0, 31, 31 }} ) };
        cell->setInheritedOpacity(0.75f);
        cell->setSize(cell->getSize() * 1.2f);
        cell->setPosition({ "parent.picture.position + 26 + (size + 4) * " + std::to_string(i) },
                          "parent.picture.position + 22");
        hotBar->add(cell, "cell" + std::to_string(i));
    }

    // general group to hold pictures of items visible in hot bar
    tgui::Group::Ptr items{ tgui::Group::create() };
    hotBar->add(items, "items");
    hotBar->setVisible(false);
    gui_.add(hotBar, "hotBar");
}

void Interface::constructHealthBar() {
    tgui::Group::Ptr healthBar{ tgui::Group::create() };
    gui_.add(healthBar, "healthBar");
}

void Interface::constructInventory() {
    tgui::Group::Ptr inventory{ tgui::Group::create() };
    tgui::Picture::Ptr inventoryPicture{ tgui::Picture::create(ResourceManager::getTexture(ResourceManager::INVENTORY)) };
    inventoryPicture->setOrigin(0.5f, 0.5f);
    inventoryPicture->setPosition("parent.size / 2");
    inventory->add(inventoryPicture, "inventoryPicture");
    inventory->setVisible(false);
    gui_.add(inventory, "inventory");
}

void Interface::constructConsole() {
    // general group to hold console related widgets
    tgui::Group::Ptr console{ tgui::Group::create() };

    tgui::EditBox::Ptr consoleInput{ tgui::EditBox::create() };
    consoleInput->onReturnKeyPress([this]() {
        consoleEnterSignal.emit();
    });

    console->add(consoleInput, "input");
    console->setVisible(false);
    gui_.add(console, "console");
}
