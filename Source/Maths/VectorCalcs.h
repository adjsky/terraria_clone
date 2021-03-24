//
// Created by adjsky on 1/10/21.
//

#ifndef TERRARIA_CLONE_VECTORCALCS_H
#define TERRARIA_CLONE_VECTORCALCS_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace math
{
template<typename T>
inline T getVectorLength(const sf::Vector2<T>& vector)
{
    return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2));
}

template<typename T>
inline T distanceBetween(const sf::Vector2<T>& from, const sf::Vector2<T>& to)
{
    return std::sqrt(std::pow(to.x - from.x, 2) + std::pow(to.y - from.y, 2));
}

inline sf::Vector2f normalize(sf::Vector2f vector)
{
    float length{getVectorLength(vector)};
    vector.x /= length;
    vector.y /= length;
    return vector;
}
}


#endif //TERRARIA_CLONE_VECTORCALCS_H
