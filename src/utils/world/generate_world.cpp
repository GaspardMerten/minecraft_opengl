//
// Created by gaspa on 27/12/2022.
//

#include <map>
#include "../../world/World.h"


World generateFlatWorld(int length, int width, int depth) {
    std::map<std::tuple<int, int, int>, int> map;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < depth; k++) {
                map[std::make_tuple(i, j, -k)] = 1;
            }
        }
    }

    return World(map);
}