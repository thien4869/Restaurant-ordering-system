#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H
#include "User.h"
struct Administrator : User {
    Administrator() { role = "admin"; }
};
#endif
