#pragma once

#include "user.h"
#include "myhash.h"
#include <random>

int GenerateIntValue(int min_val, int max_val);
void GenerateUsers(std::vector<User> &, int amount);