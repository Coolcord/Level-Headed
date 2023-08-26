#ifndef LEVEL_H
#define LEVEL_H

namespace Level {
    enum Level {
        WORLD_1_LEVEL_1 = 0,
        WORLD_1_LEVEL_2 = 1,
        WORLD_1_LEVEL_3 = 2,
        WORLD_1_LEVEL_4 = 3,
        WORLD_2_LEVEL_1 = 4,
        WORLD_2_LEVEL_2 = 5,
        WORLD_2_LEVEL_3 = 6,
        WORLD_2_LEVEL_4 = 7,
        WORLD_3_LEVEL_1 = 8,
        WORLD_3_LEVEL_2 = 9,
        WORLD_3_LEVEL_3 = 10,
        WORLD_3_LEVEL_4 = 11,
        WORLD_4_LEVEL_1 = 12,
        WORLD_4_LEVEL_2 = 13,
        WORLD_4_LEVEL_3 = 14,
        WORLD_4_LEVEL_4 = 15,
        WORLD_5_LEVEL_1 = 16,
        WORLD_5_LEVEL_2 = 17,
        WORLD_5_LEVEL_3 = WORLD_1_LEVEL_3,
        WORLD_5_LEVEL_4 = WORLD_2_LEVEL_4,
        WORLD_6_LEVEL_1 = 18,
        WORLD_6_LEVEL_2 = 19,
        WORLD_6_LEVEL_3 = 20,
        WORLD_6_LEVEL_4 = WORLD_1_LEVEL_4,
        WORLD_7_LEVEL_1 = 21,
        WORLD_7_LEVEL_2 = WORLD_2_LEVEL_2,
        WORLD_7_LEVEL_3 = WORLD_2_LEVEL_3,
        WORLD_7_LEVEL_4 = 22,
        WORLD_8_LEVEL_1 = 23,
        WORLD_8_LEVEL_2 = 24,
        WORLD_8_LEVEL_3 = 25,
        WORLD_8_LEVEL_4 = 26,
        PIPE_INTRO = 27,
        UNDERGROUND_BONUS = 28,
        CLOUD_BONUS_1 = 29,
        CLOUD_BONUS_2 = 30,
        UNDERWATER_BONUS = 31,
        WARP_ZONE = 32,
        UNDERWATER_CASTLE = 33,
        PIPE_EXIT_SMALL_CASTLE = WORLD_3_LEVEL_2,
        PIPE_EXIT_BIG_CASTLE = WORLD_1_LEVEL_3,
        CLOUD_BONUS_DAY = CLOUD_BONUS_1,
        CLOUD_BONUS_NIGHT = CLOUD_BONUS_2,
        PIPE_INTRO_SMALL_CASTLE = PIPE_INTRO,
        PIPE_INTRO_BIG_CASTLE = WARP_ZONE
    };
}

#endif // LEVEL_H
