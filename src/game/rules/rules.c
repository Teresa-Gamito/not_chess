#include "include/game/rules/rules.h"
#include "data_structures/vector.h"
#include <SDL3/SDL_stdinc.h>

RuleList* rulelist_create()
{
    return vector_create();
}

void rulelist_destroy(RuleList* rulelist)
{
    for (int i = 0; i < vector_get_size(rulelist); i++)
    {
        Rule* rule = vector_get_at(rulelist, i);
        SDL_free(rule);
    }
    SDL_free(rulelist);
}

void rulelist_add(RuleList* rulelist, Rule rule)
{
    Rule* add_rule = SDL_malloc(sizeof(Rule));
    *add_rule = rule;
    vector_add(rulelist, add_rule);
}

bool rulelist_has(const RuleList* rulelist, Rule rule)
{
    for (int i = 0; i < vector_get_size(rulelist); i++)
    {
        Rule curr = *(Rule*)vector_get_at(rulelist, i);
        if (curr == rule)
        {
            return true;
        }
    }
    return false;
}

const char* rulelist_get_rules(const RuleList* rulelist)
{
    char* rules = "";

    for (int i = 0; i < vector_get_size(rulelist); i++)
    {
        char* rule_description;

        Rule rule = *(Rule*)vector_get_at(rulelist, i);
        switch (rule)
        {
            case RULE_PAWN_PROMOTION_CHANCE:
                rule_description = RULE_DESCRIPTION_PAWN_PROMOTION_CHANCE;
                break;
            case RULE_CAPTURE_OWN:
                rule_description = RULE_DESCRIPTION_CAPTURE_OWN;
                break;
            case RULE_MOVE_CHANCE:
                rule_description = RULE_DESCRIPTION_MOVE_CHANCE;
                break;
            case RULE_PAWN_MOVE_MORE:
                rule_description = RULE_DESCRIPTION_PAWN_MOVE_MORE;
                break;
            case RULE_MORE_POINTS:
                rule_description = RULE_DESCRIPTION_MORE_POINTS;
                break;
            case RULE_RANDOM_SACRIFICE:
                rule_description = RULE_DESCRIPTION_RANDOM_SACRIFICE;
                break;
            default:
                break;
        }
        SDL_asprintf(&rules, "%s\n%s", rules, rule_description);
    }
    return rules;
}
