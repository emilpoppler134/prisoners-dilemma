#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdlib.h>
#include <stdbool.h>

typedef enum action_e
{
    DEFECT,
    COOPERATE
} action_e;

typedef struct strategy_t
{
    action_e (*algorithm)(int, action_e*, action_e*);
    bool nice;
    char* name;
    char* description;
} strategy_t;

typedef struct player_t
{
    int score;
    action_e *history;
    strategy_t *strategy;
} player_t;

typedef struct result_t
{
    strategy_t *player_strategy;
    strategy_t *opponent_strategy;
    int player_score;
    int opponent_score;
    int average_score;
} result_t;

player_t init_player(int, strategy_t*);
action_e opposite(action_e);

#endif
