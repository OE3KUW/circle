//
// Created by dagra on 03/11/2022.
//

#include "direction_t.h"

direction_t::direction_t(int _l, int _r) {
    l = _l;
    r = _r;
}

direction_t direction_t::operator+(direction_t &d) {
    return {l + d.l, r + d.r};
}

bool direction_t::operator==(direction_t &d) const {
    return (l == d.l && r == d.r);
}

void direction_t::selfcp(direction_t &source) {
    l = source.l;
    r = source.r;
}

direction_t direction_t::getUnit() const {
    direction_t cp = direction_t();
    if (l > 0) {
        cp.l = 1;
    } else if (l < 0) {
        cp.l = -1;
    } else {
        cp.l = 0;
    }
    if (r > 0) {
        cp.r = 1;
    } else if (l < 0) {
        cp.r = -1;
    } else {
        cp.r = 0;
    }
    return cp;
}

String direction_t::toString() {
    String msg = "direction_t: ";
    msg.concat(l);
    msg.concat(", ");
    msg.concat(r);
    msg.concat(" ");
    return msg;
}