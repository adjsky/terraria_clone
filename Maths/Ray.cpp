//
// Created by adjsky on 1/11/21.
//

#include "Ray.h"

#include <iostream>

namespace math {
    Ray::Ray(const sf::Vector2f& from, const sf::Vector2f& direction) :
        from_ { from },
        direction_ { direction }
    {
        if (getVectorLength(direction_) != 1) {
            direction_ = normalize(direction_);
        }
        to_ = sf::Vector2f{ direction_.x + from_.x, direction_.y + from.y };
    }

    float Ray::getLength() {
        return distanceBetween(from_, to_);
    }

    void Ray::lengthen(float factor) {
        to_.x += direction_.x * factor;
        to_.y += direction_.y * factor;
    }

    const sf::Vector2f& Ray::getPosition() {
        return to_;
    }

}