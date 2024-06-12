#include "permission.h"
#include "user.h"
#include <iostream>
using namespace std;

class Auth {
private:
  static User *auth;

public:
  static User *login(string, string);
  static void logout();
  static User *whoami();

  static bool authorize(const Permission &);
};

// ---------------------------------
User *Auth::auth = NULL;

User *Auth::login(string username, string password) {

  for (int i = 0; users[i] != NULL; i++) {

    if (users[i]->checkAuth(username, password)) {
      Auth::auth = users[i];
      return users[i];
    }
  }

  return NULL;
}

void Auth::logout() { Auth::auth = NULL; }

User *Auth::whoami() { return Auth::auth; }

bool Auth::authorize(const Permission &p) {
  User *currentUser = Auth::whoami();

  if (currentUser == NULL) {
    return false;
  }

  return currentUser->hasPermission(&p);
}
