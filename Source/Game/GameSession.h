//
// Created by adjsky on 18.01.2021.
//

#ifndef TERRARIA_CLONE_GAMESESSION_H
#define TERRARIA_CLONE_GAMESESSION_H

#include <boost/serialization/serialization.hpp>

#include "../Interface/Interface.h"
#include "../World/World.h"

class GameSession {
public:
    GameSession();

    void setPlayer(Player& player);
    void updatePlayer();
    void setWorld(World& world);

    Player& getPlayer();
    World& getWorld();

    const Player& getPlayer() const;
    const World& getWorld() const;

private:
    Player player_;
    World world_;

    // serialization
    friend boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int version) {
        ar & world_;
        ar & player_;
    }
};

#endif //TERRARIA_CLONE_GAMESESSION_H