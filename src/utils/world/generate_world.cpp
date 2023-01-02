//
// Created by gaspa on 27/12/2022.
//

#include <map>
#include "../../world/World.h"


World* generateFlatWorld(int length, int width, int depth) {
    std::map<std::tuple<int, int, int>, int> map;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < depth; k++) {
                map[std::make_tuple(i, j, -k)] = 1;
            }
        }
    }

    // add higher blocks only  on the edges
    for (int i = 0; i < length; i++) {
        map[std::make_tuple(i, width, 1)] = 1;
        map[std::make_tuple(i, 0, 1)] = 1;
    }
    // add higher blocks only  on the edges
    for (int i = 0; i < width; i++) {
        map[std::make_tuple(length, i, 1)] = 1;
        map[std::make_tuple(0, i, 1)] = 1;
    }

    return new World(map, length, width, depth);
}