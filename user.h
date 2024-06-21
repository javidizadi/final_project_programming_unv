#pragma once
#include "permission.h"
#include <iostream>
using namespace std;

const size_t MAX_USER_PERMISSIONS = 100;
const size_t MAX_USERS = 100;

class User {
private:
  string name, username, password;
  Permission *permissions[MAX_USER_PERMISSIONS] = {NULL};

public:
  User(string, string, string);
  static User *create(string, string, string, const User *);
  void print();
  bool checkAuth(string, string);
  void addPermission(Permission *);

  string getName();
  string getUsername();
  bool hasPermission(const Permission *) const;
  bool hasPermission(string) const;
  void addToPointerArray(User *[], size_t);
  static void list();
};

// ---------------------------------
User *users[MAX_USERS] = {NULL};

User::User(string n, string u, string p) : name(n), username(u), password(p) {}

User *User::create(string n, string u, string p, const User *currentUser) {

  // Can't use authorize method. Checking manually ( Dependency cycle problem )
  if (currentUser == NULL ||
      currentUser->hasPermission(&ADD_USER_PERM) == false) {
    return NULL;
  }

  User *user = new User(n, u, p);
  user->addToPointerArray(users, MAX_USERS);

  return user;
}

void User::print() {
  cout << "Name: " << this->name << endl
       << "Username: " << this->username << endl;

  cout << "Permissions:" << endl;
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    cout << '\t';
    this->permissions[i]->print();
    cout << endl;
  }
}

bool User::checkAuth(string username, string password) {
  for (size_t i = 0; users[i] != NULL; i++) {
    if (users[i]->username == username && users[i]->password == password) {
      return true;
    }
  }
  return false;
}

void User::addPermission(Permission *p) {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (*this->permissions[i] == *p)
      return;
  }
  p->addToPointerArray(this->permissions, MAX_USER_PERMISSIONS);
}

string User::getName() { return this->name; }

bool User::hasPermission(const Permission *p) const {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (*this->permissions[i] == *p)
      return true;
  }
  return false;
}

bool User::hasPermission(string title) const {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (this->permissions[i]->getTitle() == title)
      return true;
  }
  return false;
}

void User::addToPointerArray(User *arr[], size_t arrSize) {

  if (arr[arrSize - 1] != NULL)
    return;

  int i = 0;
  while (arr[i] != NULL) {
    i++;
  }
  arr[i] = this;
}

string User::getUsername() { return username; }

void User::list() {
  for (size_t i = 0; users[i] != NULL; i++) {
    cout << "------------------------------------" << endl;
    users[i]->print();
    cout << "------------------------------------" << endl;
  }
}
