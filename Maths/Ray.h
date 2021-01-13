//
// Created by adjsky on 1/11/21.
//

#ifndef TERRARIA_CLONE_RAY_H
#define TERRARIA_CLONE_RAY_H

#include <SFML/System/Vector2.hpp>
#include "../Maths/Math.h"

namespace math {
    class Ray {
    public:
        Ray(const sf::Vector2f& from, const sf::Vector2f& direction);
        float getLength();
        void lengthen(float factor);
        const sf::Vector2f& getPosition();

    private:
        sf::Vector2f from_;
        sf::Vector2f direction_;
        sf::Vector2f to_;
    };

}

#endif //TERRARIA_CLONE_RAY_H