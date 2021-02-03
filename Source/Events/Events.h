//
// Created by adjsky on 03.02.2021.
//

#ifndef TERRARIA_CLONE_EVENTS_H
#define TERRARIA_CLONE_EVENTS_H

#include <string>
#include <SFML/System.hpp>

namespace GameEvent {
    struct HotBarCellPressed {
        int x;
    };

    struct InventoryCellPressed {
        int x;
        int y;
    };

    struct ConsoleEntered {
        std::string data;
    };

    struct InventoryShown {
    };

    struct ConsoleShown {
    };

    struct BlockBroken {
        sf::Vector2i blockPosition;
    };

    struct BlockPlaced {
        sf::Vector2i blockPosition;
    };

    struct NoClipSet {
    };

    struct HitBoxesDrawn {
    };

    struct HotBarSwitched {
        int x;
    };
}

#endif //TERRARIA_CLONE_EVENTS_H
