#include "include/common.h"
#include "include/player.h"
#include "include/strategies.h"

#include "player.c"
#include "strategies.c"

char* translate_action(action_e action)
{
    return action == COOPERATE ? "Cooperate" : "Defect";
}

void swap(result_t* xp, result_t* yp)
{
    result_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void runScenario(int turns, player_t *player_1, player_t *player_2)
{
    int R = 3;
    int P = 1;
    int S = 0;
    int T = 5;

    for (int i = 0; i < turns; i++)
    {
        action_e p1_action = player_1->strategy->algorithm(i, player_1->history, player_2->history);
        action_e p2_action = player_2->strategy->algorithm(i, player_2->history, player_1->history);

        player_1->history[i] = p1_action;
        player_2->history[i] = p2_action;

        if (p1_action == COOPERATE && p2_action == COOPERATE)
        {
            player_1->score += R;
            player_2->score += R;
        }
        else if (p1_action == COOPERATE && p2_action == DEFECT)
        {
            player_1->score += S;
            player_2->score += T;
        }
        else if (p1_action == DEFECT && p2_action == COOPERATE)
        {
            player_1->score += T;
            player_2->score += S;
        }
        else
        {
            player_1->score += P;
            player_2->score += P;
        }
    }
}

int main(int argc, char** argv)
{
    int turns = argc > 1 ? atoi(argv[1]) : 200;

    srand(time(NULL));

    strategy_t strategies[] = {
        {&always_cooperate, true, "Always Cooperate", "Always cooperating."},
        {&always_defect, false, "Always Defect", "Always defecting."},
        {&random_move, false, "Random", "Make a random move."},
        {&alternate, false, "Alternate", "Start by cooperating, then cooperate and defect alternately."},
        {&appease, true, "Appease", "Start by cooperating, then repeat your previous move if the other player has cooperated or do the opposite if they have defected."},
        {&copy_average, true, "Copy Average", "Choose a random move, but with a probability distribution that matches the other player\'s move distribution. In other words, if the other player has cooperated for 20% of the time, cooperate with a probability of 20%."},
        {&grim_trigger, true, "Grim Trigger", "Cooperate until the other player defects, after that always defect."},
        {&pavlovian, true, "Pavlovian", "Start by cooperating, then repeat the previous move if had a positive outcome or do the opposite otherwise."},
        {&joss, false, "Joss", "Start by cooperating, if the other player chooses cooperation, Joss has a 90% probability of choosing cooperation on its next move. If the other player chooses defection, Joss chooses defection on its next move."},
        {&feld, false, "Feld", "Uses a tit-for-tat strategy at first. When the other player chooses cooperation, Feld gradually reduces the probability of cooperation being selected. When the other player chooses defection, Feld chooses defection on its next move."},
        {&graaskamp, false, "Graaskamp", "Uses a tit-for-tat strategy for the first 50 moves. Thereafter, every 10 move Grasskamp has a 50% probability to defect."},
        {&tit_for_tat, true, "Tit For Tat", "Start by cooperating, then copy the other player\'s moves."},
        {&reverse_tit_for_tat, false, "Reverse Tit For Tat", "Start by defecting, then copy the other player\'s moves."},
        {&tit_for_two_tats, true, "Tit For Two Tats", "Always cooperate, unless the other player has defected the last two times."},
        {&two_tits_for_tat, true, "Two Tits For Tat", "Always cooperate, unless the other player has betrayed at least once in the last two moves."}
    };

    int strategy_arr_length = sizeof(strategies) / sizeof(strategy_t);

    int option;

    while (true)
    {
        printf("1. Run a tournament\n");
        printf("2. Run one strategy against all\n");
        printf("3. Run two strategies against each other\n");
        printf("Choose a option: ");
        scanf("%d", &option);

        if (option > 0 && option <= 3)
        {
            break;
        }
        else
        {
            printf("Not a valid option.\n");
        }
    }

    puts("");

    switch (option)
    {
        case 1:
        {
            result_t result[strategy_arr_length];

            for (int i = 0; i < strategy_arr_length; i++)
            {
                float total = 0.0;

                for (int j = 0; j < strategy_arr_length; j++)
                {
                    player_t player = init_player(turns, &strategies[i]);
                    player_t opponent = init_player(turns, &strategies[j]);

                    runScenario(turns, &player, &opponent);

                    total += player.score;

                    free(player.history);
                    free(opponent.history);
                }

                result[i] = (result_t){ &strategies[i], NULL, 0, 0, floor((total / strategy_arr_length)) };
            }

            int min_idx;

            for (int i = 0; i < strategy_arr_length - 1; i++)
            {
                min_idx = i;

                for (int j = i + 1; j < strategy_arr_length; j++)
                {
                    if (result[j].average_score > result[min_idx].average_score)
                    {
                        min_idx = j;
                    }
                }
                swap(&result[min_idx], &result[i]);
            }

            for (int i = 0; i < strategy_arr_length; i++)
            {
                result_t *r = &result[i];
                int name_length = strlen(r->player_strategy->name);
                printf("%s: %*s %c    %d\n", r->player_strategy->name, 42 - name_length, " ", r->player_strategy->nice ? 'N' : 'D', r->average_score);
            }

            break;
        }

        case 2:
        {
            int choice;

            while (true)
            {
                for (int i = 0; i < strategy_arr_length; i++)
                {
                    printf("%d. %s\n", i + 1, strategies[i].name);
                }

                printf("Choose a strategy: ");
                scanf("%d", &choice);

                if (choice > 0 && choice < strategy_arr_length + 1)
                {
                    break;
                }
                else
                {
                    printf("The choice must be c > 0 and c < %d.\n", strategy_arr_length + 1);
                }
            }

            puts("");

            strategy_t player_strategy = strategies[choice - 1];
            result_t result[turns];

            for (int i = 0; i < strategy_arr_length; i++)
            {
                player_t player = init_player(turns, &player_strategy);
                player_t opponent = init_player(turns, &strategies[i]);

                runScenario(turns, &player, &opponent);

                result[i] = (result_t){ player.strategy, opponent.strategy, player.score, opponent.score, 0 };

                free(player.history);
                free(opponent.history);
            }

            int min_idx;

            for (int i = 0; i < strategy_arr_length - 1; i++)
            {
                min_idx = i;

                for (int j = i + 1; j < strategy_arr_length; j++)
                {
                    if (result[j].player_score > result[min_idx].player_score)
                    {
                        min_idx = j;
                    }
                }
                swap(&result[min_idx], &result[i]);
            }

            for (int i = 0; i < strategy_arr_length; i++)
            {
                result_t *r = &result[i];
                int name_length = strlen(r->player_strategy->name) + strlen(r->opponent_strategy->name);
                printf("%s vs %s: %*s %c-%c    %d-%d\n", r->player_strategy->name, r->opponent_strategy->name, 42 - name_length, " ", r->player_strategy->nice ? 'N' : 'D', r->opponent_strategy->nice ? 'N' : 'D', r->player_score, r->opponent_score);
            }

            break;
        }

        case 3:
        {
            int player_choice;

            while (true)
            {
                for (int i = 0; i < strategy_arr_length; i++)
                {
                    printf("%d. %s\n", i + 1, strategies[i].name);
                }

                printf("Choose a player strategy: ");
                scanf("%d", &player_choice);

                if (player_choice > 0 && player_choice < strategy_arr_length + 1)
                {
                    break;
                }
                else
                {
                    printf("The choice must be c > 0 and c < %d.\n", strategy_arr_length + 1);
                }
            }

            puts("");

            int opponent_choice;

            while (true)
            {
                for (int i = 0; i < strategy_arr_length; i++)
                {
                    printf("%d. %s\n", i + 1, strategies[i].name);
                }

                printf("Choose a opponent strategy: ");
                scanf("%d", &opponent_choice);

                if (opponent_choice > 0 && opponent_choice < strategy_arr_length + 1)
                {
                    break;
                }
                else
                {
                    printf("The choice must be c > 0 and c < %d.\n", strategy_arr_length + 1);
                }
            }

            puts("");

            result_t result;

            strategy_t player_strategy = strategies[player_choice - 1];
            strategy_t opponent_strategy = strategies[opponent_choice - 1];

            player_t player = init_player(turns, &player_strategy);
            player_t opponent = init_player(turns, &opponent_strategy);

            runScenario(turns, &player, &opponent);

            result = (result_t){ player.strategy, opponent.strategy, player.score, opponent.score, 0 };

            free(player.history);
            free(opponent.history);

            int name_length = strlen(result.player_strategy->name) + strlen(result.opponent_strategy->name);
            printf("%s vs %s: %*s %c-%c    %d-%d\n", result.player_strategy->name, result.opponent_strategy->name, 42 - name_length, " ", result.player_strategy->nice ? 'N' : 'D', result.opponent_strategy->nice ? 'N' : 'D', result.player_score, result.opponent_score);

            break;
        }
    }

    return 0;
}
