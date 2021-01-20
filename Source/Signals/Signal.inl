//
// Created by adjsky on 20.01.2021.
//

#include "Signal.h"

template<class... Types>
Signal<Types...>::Signal() :
    callbacks_{ }
{

}

template<class... Types>
void Signal<Types...>::connect(std::function<void(Types...)> callback) {
    callbacks_.push_back(callback);
}

template<class... Types>
void Signal<Types...>::emit(Types... data) {
    for (const auto& func : callbacks_) {
        std::invoke(func, data...);
    }
}
