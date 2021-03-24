//
// Created by adjsky on 06.02.2021.
//

#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "GameSerialization.h"

void GameSerialization::saveGame(const GameSession& gameSession)
{
    SerializedData serializedData;
    serializedData.player = gameSession.getPlayer();
    serializedData.world = gameSession.getWorld();
    std::ofstream ofstr{"Game.data"};
    boost::archive::text_oarchive oa{ofstr};
    oa << serializedData;
}

GameSerialization::SerializedData GameSerialization::getGameData()
{
    SerializedData data;
    std::ifstream ifstr{"Game.data"};
    boost::archive::text_iarchive ia{ifstr};
    ia >> data;
    data.world.updateSprites();
    return data;
}

bool GameSerialization::isGameSaved()
{
    std::ifstream ifstream{"Game.data"};
    return ifstream.is_open();
}