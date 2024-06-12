#include "permission.h"
#include "question_base.h"
#include "tag.h"
#include "user.h"

#include <iostream>
using namespace std;

void initAdminUser();

// Used in first of main function.
void init() { initAdminUser(); }

void initAdminUser() {
  User *admin = new User("admin", "admin", "123456");

  for (size_t i = 0; permissions[i] != NULL; i++) {
    admin->addPermission(permissions[i]);
  }

  admin->addToPointerArray(users, MAX_USERS);
}

void freeMemories() {

  for (size_t i = 0; permissions[i] != NULL; i++)
    delete permissions[i];

  for (size_t i = 0; users[i] != NULL; i++)
    delete users[i];

  for (size_t i = 0; tags[i] != NULL; i++)
    delete tags[i];

  for (size_t i = 0; questions[i] != NULL; i++)
    delete questions[i];
}
