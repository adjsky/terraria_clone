//
// Created by adjsky on 06.02.2021.
//

#ifndef TERRARIA_CLONE_GAMESERIALIZATION_H
#define TERRARIA_CLONE_GAMESERIALIZATION_H

#include <fstream>
#include <SFML/System/Vector2.hpp>
#include <boost/serialization/serialization.hpp>
#include "../../Game/GameSession.h"
#include "../../Entities/Player/Player.h"

namespace boost::serialization {
    template<class Archive>
    void serialize(Archive & ar, sf::Vector2i vector, const unsigned int version)
    {
        ar & vector.x;
        ar & vector.y;
    }
    template<class Archive>
    void serialize(Archive & ar, sf::Vector2f vector, const unsigned int version)
    {
        ar & vector.x;
        ar & vector.y;
    }
}

namespace GameSerialization {
    struct SerializedData {
        World world;
        Player player;

        // serialization
        friend boost::serialization::access;
        template<class Archive>
        inline void serialize(Archive & ar, const unsigned int version) {
            ar & world;
            ar & player;
        }
    };

    void saveGame(const GameSession& gameSession);
    SerializedData getGameData();
    bool isGameSaved();
}

#endif //TERRARIA_CLONE_GAMESERIALIZATION_H
