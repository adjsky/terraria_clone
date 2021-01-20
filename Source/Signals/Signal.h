//
// Created by adjsky on 20.01.2021.
//

#ifndef TERRARIA_CLONE_SIGNAL_H
#define TERRARIA_CLONE_SIGNAL_H

#include <vector>
#include <functional>

template<class... Types>
class Signal {
public:
    Signal();

    void connect(std::function<void(Types...)> callback);
    void emit(Types... data);

private:
    std::vector<std::function<void(Types...)>> callbacks_;
};

#include "Signal.inl"

#endif //TERRARIA_CLONE_SIGNAL_H