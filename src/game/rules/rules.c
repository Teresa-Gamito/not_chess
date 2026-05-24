#include "include/game/rules/rules.h"

struct Rules 
{
    Vector* rules;
};

RuleList* rulelist_create()
{
    return vector_create();
}

void rulelist_destroy(RuleList* rulelist)
{
    for (int i = 0; i < vector_get_size(rulelist); i++)
    {
        SDL_free(vector_get_at(rulelist, i));
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
