#ifndef __STRATEGIES_H__
#define __STRATEGIES_H__

#include <math.h>
#include <stdbool.h>

#include "player.h"

action_e always_cooperate(int, action_e*, action_e*);

action_e always_defect(int, action_e*, action_e*);

action_e random_move(int, action_e*, action_e*);

action_e alternate(int, action_e*, action_e*);

action_e appease(int, action_e*, action_e*);

action_e copy_average(int, action_e*, action_e*);

action_e grim_trigger(int, action_e*, action_e*);

action_e pavlovian(int, action_e*, action_e*);

action_e joss(int, action_e*, action_e*);

action_e feld(int, action_e*, action_e*);

action_e graaskamp(int, action_e*, action_e*);

action_e tit_for_tat(int, action_e*, action_e*);

action_e reverse_tit_for_tat(int, action_e*, action_e*);

action_e tit_for_two_tats(int, action_e*, action_e*);

action_e two_tits_for_tat(int, action_e*, action_e*);

#endif
