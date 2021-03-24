//
// Created by adjsky on 18.01.2021.
//

#include "GameSession.h"
#include "../Core/Engine.h"

GameSession::GameSession() :
    player_{ },
    world_{ }
{
    updatePlayer();
}

void GameSession::setPlayer(Player& player)
{
    player_ = player;
    updatePlayer();
}

void GameSession::updatePlayer()
{
    player_.setOrigin(PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f);
    player_.setScale(static_cast<float>(BLOCK_SIZE) / PLAYER_WIDTH * 1.5f, static_cast<float>(BLOCK_SIZE) / PLAYER_HEIGHT * 3);
    player_.setTimeStep(0.2f);
    player_.constructHitBox();

    Animation& moveAnimation{ player_.getAnimation(Player::MOVE) };
    moveAnimation.setSpriteSheet(Engine::getResourceManager()->getTexture(ResourceManager::PLAYER));
    moveAnimation.addFrame(sf::IntRect{ 65, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    moveAnimation.addFrame(sf::IntRect{ 119, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 172, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 226, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
    moveAnimation.addFrame(sf::IntRect{ 280, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    Animation& standAnimation{ player_.getAnimation(Player::STAND) };
    standAnimation.setSpriteSheet(Engine::getResourceManager()->getTexture(ResourceManager::PLAYER));
    standAnimation.addFrame(sf::IntRect{ 9, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    Animation& jumpAnimation{ player_.getAnimation(Player::JUMP) };
    jumpAnimation.setSpriteSheet(Engine::getResourceManager()->getTexture(ResourceManager::PLAYER));
    jumpAnimation.addFrame(sf::IntRect{ 383, 0, PLAYER_WIDTH, PLAYER_HEIGHT });

    player_.setAnimation(standAnimation);

    Inventory::Cell cell{ };
    cell.itemType = ItemTypes::SWORD;
    cell.id = static_cast<int>(SwordType::IRON);
    cell.amount = 1;
    player_.getHotBar().addItem(cell);

    Interface* gui{ Engine::getInterface() };
    gui->updateHealth(player_);
    gui->updateHotBar(player_);
    gui->updateInventory(player_);
    gui->highlightHotBarCell(player_);
}

void GameSession::setWorld(World& world)
{
    world_ = world;
}

Player& GameSession::getPlayer()
{
    return player_;
}

World& GameSession::getWorld()
{
    return world_;
}

const Player& GameSession::getPlayer() const
{
    return player_;
}

const World& GameSession::getWorld() const
{
    return world_;
}