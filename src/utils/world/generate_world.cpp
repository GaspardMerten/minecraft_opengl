//
// Created by gaspa on 27/12/2022.
//

#include <map>
#include "../../world/World.h"


World* generateFlatWorld(int length, int width, int depth, int nbrTrees, int nbCircles) {
    std::map<std::tuple<int, int, int>, std::tuple<int, MeshType, TextureType>> map;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < depth; k++) {
                map[std::make_tuple(i, j, -k)] = std::make_tuple(1, MeshType::CUBEMAP, TextureType::GRASS);
            }
        }
    }

    int waterCenterX, waterCenterZ;
    // randomly choose the center of the water, but not too close to the edges

    for (int circle = 0; circle < nbCircles; circle++) {
        waterCenterX = rand() % (length - 20) + 10;
        waterCenterZ = rand() % (width - 20) + 10;
        int waterRadius = rand() % 5 + 5;

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                if (pow(i - waterCenterX, 2) + pow(j - waterCenterZ, 2) < pow(waterRadius, 2)) {
                    map[std::make_tuple(i, j, 0)] = std::make_tuple(1, MeshType::PLANE, TextureType::WATER);
                    map[std::make_tuple(i, j, -1)] = std::make_tuple(1, MeshType::CUBEMAP, TextureType::DIRT);
                }
            }
        }
    }



    // add higher blocks only  on the edges
    for (int i = 0; i < length; i++) {
        map[std::make_tuple(i, width, 1)] = std::make_tuple(1, MeshType::CUBEMAP, TextureType::GRASS);;
        map[std::make_tuple(i, 0, 1)] = std::make_tuple(1, MeshType::CUBEMAP, TextureType::GRASS);;
    }
    // add higher blocks only  on the edges
    for (int i = 0; i < width; i++) {
        map[std::make_tuple(length, i, 1)] = std::make_tuple(1, MeshType::BLOCK, TextureType::DIRT);;
        map[std::make_tuple(0, i, 1)] = std::make_tuple(1,MeshType::BLOCK, TextureType::DIRT);;
    }

    // add little trees randomly
    for (int i = 0; i < nbrTrees; i++) {
        int x = rand() % length;
        int y = rand() % width;
        for(int j = 0; j < 5; j++) {
            map[std::make_tuple(x, y, j)] = std::make_tuple(1, MeshType::BLOCK, TextureType::WOOD);;
        }

        // Leaves
        for(int j = 0; j < 5; j++) {
            for(int k = 0; k < 5; k++) {
                map[std::make_tuple(x - 2 + j, y - 2 + k, 5)] = std::make_tuple(1, MeshType::BLOCK, TextureType::LEAF);;
            }
        }
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                map[std::make_tuple(x - 1 + j, y - 1 + k, 6)] = std::make_tuple(1, MeshType::BLOCK, TextureType::LEAF);;
            }
        }
        map[std::make_tuple(x, y, 7)] = std::make_tuple(1, MeshType::BLOCK, TextureType::LEAF);;


    }

    return new World(map, length, width, depth);
}