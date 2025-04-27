#pragma once

#include "enums.h"

struct FoodPreference
{
    DietCategory dietCategory;
    SpiceLevel spiceLevel;
    CuisineCategory cuisineCategory;
    bool sweetTooth;

    FoodPreference() = default;
    FoodPreference(DietCategory dietCategory, SpiceLevel spiceLevel, CuisineCategory cuisineCategory, bool sweetTooth)
        : dietCategory(dietCategory), spiceLevel(spiceLevel), cuisineCategory(cuisineCategory), sweetTooth(sweetTooth) {}
};