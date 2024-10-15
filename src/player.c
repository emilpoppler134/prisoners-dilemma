#include "include/player.h"

player_t init_player(int turns, strategy_t *strategy) {
    player_t player;
    player.score = 0;
    player.history = (action_e *)malloc(turns * sizeof(action_e));
    player.strategy = strategy;
    return player;
}

action_e opposite(action_e move)
{
    return move == COOPERATE ? DEFECT : COOPERATE;
}
