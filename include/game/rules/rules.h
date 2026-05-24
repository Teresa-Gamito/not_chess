#pragma once

#include "data_structures/vector.h"

#define CHANCE_RULE_PAWN_PROMOTE 0.2

typedef enum Rule 
{
    RULE_PAWN_MOVE_MORE,
    RULE_PAWN_PROMOTION_CHANCE,
    RULE_RANDOM_SACRIFICE,
    RULE_CAPTURE_OWN,
    RULE_MORE_POINTS,
    RULE_MOVE_CHANCE,
} Rule;

typedef Vector RuleList;

RuleList* rulelist_create();
void rulelist_destroy(RuleList* rulelist);

void rulelist_add(RuleList* rulelist, Rule rule);
bool rulelist_has(const RuleList* rulelist, Rule rule);
