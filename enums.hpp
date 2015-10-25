#ifndef ENUMS_H
#define ENUMS_H

typedef enum {
    NORTH='n',
    EAST='e',
    SOUTH='s',
    WEST='w',
    STAY,
    TRAPPED='x'
} direction_type;

typedef enum  {
    RABBIT = 'r',
    CAT = 'c',
    DOG = 'd',
    HORSE = 'h',
    CAMEL = 'm',
    ELEPHANT = 'e'
} piece_type;

typedef enum {
    NONE,
    GOLD,
    SILVER,
    ALL
} observer_config;

#endif //ENUMS_H


