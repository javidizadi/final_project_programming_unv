#pragma once

#include <iostream>
using namespace std;

const size_t MAX_PERMISSIONS = 100;

class Permission {
  string title;

public:
  Permission(string);
  static Permission *create(string);
  string getTitle();
  void print();

  bool operator==(const Permission &);
  void addToPointerArray(Permission *[], size_t);
};

// ---------------------------------
// Hardcoded permissions
Permission ADD_DESCRIPTIVE_QUESTION_PERM("add-descriptive-question");
Permission ADD_FOUR_CHOICE_QUESTION_PERM("add-four-choice-question");
Permission ADD_USER_PERM("add-user");
Permission EDIT_DESCRIPTIVE_QUESTION_PERM("edit-descriptive-question");
Permission EDIT_FOUR_CHOICE_QUESTION_PERM("edit-four-choice-question");
Permission PUBLISH_QUESTION_PERM("publish-question");

Permission *permissions[MAX_PERMISSIONS] = {&ADD_DESCRIPTIVE_QUESTION_PERM,
                                            &ADD_FOUR_CHOICE_QUESTION_PERM,
                                            &ADD_USER_PERM,
                                            &EDIT_FOUR_CHOICE_QUESTION_PERM,
                                            &EDIT_DESCRIPTIVE_QUESTION_PERM,
                                            &PUBLISH_QUESTION_PERM};

Permission::Permission(string title) : title(title) {}

Permission *Permission::create(string title) {
  Permission *perm = new Permission(title);
  perm->addToPointerArray(permissions, MAX_PERMISSIONS);
  return perm;
}

string Permission::getTitle() { return this->title; }

void Permission::print() { cout << title; }

bool Permission::operator==(const Permission &p) {
  if (p.title == this->title) {
    return true;
  }
  return false;
}

void Permission::addToPointerArray(Permission *arr[], size_t arrSize) {
  // Ensure array isn't full.
  if (arr[arrSize - 1] != NULL)
    return;

  Permission **toWrite = arr;
  while (*toWrite != NULL)
    toWrite++;

  *toWrite = this;
}
