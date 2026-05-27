#pragma once

#include "data_structures/vector.h"

#define CHANCE_RULE_PAWN_PROMOTE 0.2

#define RULE_DESCRIPTION_PAWN_MOVE_MORE "Pawns can move up to three tiles on the first move"
#define RULE_DESCRIPTION_PAWN_PROMOTION_CHANCE "Pawns have a 30% chance being promoted to a queen when capturing"
#define RULE_DESCRIPTION_CAPTURE_OWN "Players can capture their own pieces"
#define RULE_DESCRIPTION_MORE_POINTS "Gain one more Capturing Point when capturing"
#define RULE_DESCRIPTION_MOVE_CHANCE "Players have a 30% chance of playing again after a capture"
#define RULE_DESCRIPTION_RANDOM_SACRIFICE "Every turn sacrifice one piece"

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

const char* rulelist_get_rules(const RuleList* rulelist);
