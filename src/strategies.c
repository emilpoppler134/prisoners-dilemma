#include "include/strategies.h"
#include "include/player.h"

action_e always_cooperate(int index, action_e *player_history, action_e *opponent_history)
{
    return COOPERATE;
}

action_e always_defect(int index, action_e *player_history, action_e *opponent_history)
{
    return DEFECT;
}

action_e random_move(int index, action_e *player_history, action_e *opponent_history)
{
    return rand() % 100 >= 50 ? COOPERATE : DEFECT;
}

action_e alternate(int index, action_e *player_history, action_e *opponent_history)
{
    return index % 2 == 0 ? COOPERATE : DEFECT;
}

action_e appease(int index, action_e *player_history, action_e *opponent_history)
{
   if (index == 0) return COOPERATE;

   action_e player_last = player_history[index - 1];
   action_e opponent_last = opponent_history[index - 1];

   return opponent_last == COOPERATE ? player_last : opposite(player_last);
}

action_e copy_average(int index, action_e *player_history, action_e *opponent_history)
{
    int opponent_defects = 0;

    for (int i = 0; i < index; i++)
    {
        if (opponent_history[i] == DEFECT)
        {
            opponent_defects++;
        }
    }

    float probability = opponent_defects / (index > 0 ? index : 1.0);
    int probability_p = floor(probability * 100);
    int random_p = rand() % 100;

    return random_p < probability_p ? DEFECT : COOPERATE;
}

action_e grim_trigger(int index, action_e *player_history, action_e *opponent_history)
{
    for (int i = 0; i < index; i++)
    {
        if (opponent_history[i] == DEFECT)
        {
            return DEFECT;
        }
    }

    return COOPERATE;
}

action_e pavlovian(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return COOPERATE;

    action_e player_last = player_history[index - 1];
    action_e opponent_last = opponent_history[index - 1];
    bool player_last_positive = (player_last == COOPERATE && opponent_last == COOPERATE) || (player_last == DEFECT && opponent_last == COOPERATE);

    return player_last_positive ? player_last : opposite(player_last);
}

action_e joss(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return COOPERATE;
    if (opponent_history[index - 1] == DEFECT) return DEFECT;
    return rand() % 100 < 90 ? COOPERATE : DEFECT;
}

action_e feld(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return COOPERATE;
    if (opponent_history[index - 1] == DEFECT) return DEFECT;

    int probability = 100 - (index > 100 ? 100 : index);
    return rand() % 100 < probability ? COOPERATE : DEFECT;
}

action_e graaskamp(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return COOPERATE;
    if (index < 50) return opponent_history[index - 1];

    return index + 1 % 10 == 0
        ? rand() % 100 < 50 ? COOPERATE : DEFECT
        : COOPERATE;
}

action_e tit_for_tat(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return COOPERATE;
    action_e opponent_last = opponent_history[index - 1];
    return opponent_last;
}

action_e reverse_tit_for_tat(int index, action_e *player_history, action_e *opponent_history)
{
    if (index == 0) return DEFECT;
    action_e opponent_last = opponent_history[index - 1];
    return opponent_last;
}

action_e tit_for_two_tats(int index, action_e *player_history, action_e *opponent_history)
{
    if (index < 2) return COOPERATE;
    bool opponent_last_two_defected = opponent_history[index - 1] == DEFECT && opponent_history[index - 2] == DEFECT;
    return opponent_last_two_defected ? DEFECT : COOPERATE;
}

action_e two_tits_for_tat(int index, action_e *player_history, action_e *opponent_history)
{
    if (index < 2) return COOPERATE;
    bool opponent_last_two_once_defected = opponent_history[index - 1] == DEFECT || opponent_history[index - 2] == DEFECT;
    return opponent_last_two_once_defected ? DEFECT : COOPERATE;
}
