#pragma once
#include "auth.h"
#include "date_time.h"
#include "tag.h"
#include "user.h"
#include <iostream>

using namespace std;

const size_t MAX_QUESTIONS = 100;
const size_t MAX_USER_TAGS = 100;

class Question {

protected:
  string question;
  DateTime createdAt;
  string type;
  User user;
  Tag *tags[MAX_USER_TAGS] = {NULL};
  bool isPublished = false;

public:
  Question(string, DateTime, User, string);

  void publish();
  void unPublish();
  virtual void print() = 0;
  virtual void printAll() = 0;
  void addTag(Tag *);

  static void list();

  virtual Question *edit(string question, DateTime createdAt, User user,
                         string answer) = 0;
  virtual Question *edit(string question, DateTime createdAt, User user,
                         string a, string b, string c, string d,
                         char answer) = 0;

  void addToPointerArray(Question *[], size_t);
  string getType();
  bool isAuthorUser(User &u);
};

// ---------------------------------
Question *questions[MAX_QUESTIONS] = {NULL};

Question::Question(string q, DateTime c, User u, string t)
    : question(q), createdAt(c), user(u), type(t) {}

void Question::publish() { this->isPublished = true; }

void Question::unPublish() { this->isPublished = false; }

void Question::addTag(Tag *tag) {
  // Check if have a same tag, don't add it to tags.
  for (int i = 0; this->tags[i] != NULL; i++) {
    if (*this->tags[i] == *tag) {
      return;
    }
  }
  tag->addToPointerArray(this->tags, MAX_USER_TAGS);
}

void Question::list() {
  for (size_t i = 0; questions[i] != NULL; i++) {
    cout << endl << "------------------------------------------" << endl;

    cout << "ID: " << i << endl;

    cout << "Type: " << questions[i]->type << endl;

    questions[i]->print();

    cout << endl << "------------------------------------------" << endl;
  }
}

string Question::getType() { return this->type; }

void Question::addToPointerArray(Question *arr[], size_t arrSize) {
  // Ensure array isn't full.
  if (arr[arrSize - 1] != NULL)
    return;

  Question **toWrite = arr;
  while (*toWrite != NULL)
    toWrite++;

  *toWrite = this;
}

bool Question::isAuthorUser(User &u) {

  if (this->user.getUsername() == u.getUsername()) {
    return true;
  }

  return false;
}
