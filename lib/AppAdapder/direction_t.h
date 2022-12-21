//
// Created by dagra on 03/11/2022.
//

#ifndef CIRCLE_DIRECTION_T_H
#define CIRCLE_DIRECTION_T_H
#include <Arduino.h>

class direction_t {
public:
    int l{};
    int r{};

public:
    direction_t(){}
    direction_t(int, int);
    void selfcp(direction_t&);
    bool operator== (direction_t&) const;
    direction_t operator+ (direction_t&);
    direction_t getUnit() const;
    String toString();
};


#endif //CIRCLE_DIRECTION_T_H
