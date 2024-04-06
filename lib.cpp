
#include "lib.h"

bool operator<(const Node &x, const Node &y) {
    return x.getKey() < y.getKey();
}