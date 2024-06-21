#pragma once
#include "auth.h"
#include "date_time.h"
#include "permission.h"
#include "question_base.h"
#include <iostream>

using namespace std;

class Descriptive : public Question {

private:
  string answer;

public:
  Descriptive(string, DateTime, User);
  static Descriptive *create(string, DateTime, User);
  void addAnswer(string);
  void print();
  static void printAll();
  Question *edit(string, DateTime, User, string);
  Question *edit(string, DateTime, User, string, string, string, string, char) {
    return NULL;
  }
};

// ---------------------------------
Descriptive::Descriptive(string question, DateTime createdAt, User user)
    : Question(question, createdAt, user, "descriptive") {}

void Descriptive::addAnswer(string answer) { this->answer = answer; }

Question *Descriptive::edit(string question, DateTime createdAt, User user,
                            string answer) {

  if (Auth::authorize(EDIT_DESCRIPTIVE_QUESTION_PERM) == false)
    return NULL;

  this->question = question;
  this->createdAt = createdAt;
  this->user = user;
  this->answer = answer;
  return this;
};

// Not implemented for descriptive questions (returns NULL)
Question *edit(string question, DateTime createdAt, User user, string a,
               string b, string c, string d, char answer) {
  return NULL;
};

Descriptive *Descriptive::create(string question, DateTime createdAt,
                                 User user) {

  if (Auth::authorize(ADD_DESCRIPTIVE_QUESTION_PERM) == false)
    return NULL;

  Descriptive *q = new Descriptive(question, createdAt, user);
  q->addToPointerArray(questions, MAX_QUESTIONS);
  return q;
}

void Descriptive::print() {

  if (!this->isPublished) {
    cout << "The question isn't published yet." << endl;
    return;
  }

  cout << "Created at: ";
  printDateTime(this->createdAt);
  cout << endl;

  cout << "Tags:";
  for (size_t i = 0; this->tags[i] != NULL; i++) {
    cout << ' ';
    this->tags[i]->print();
  }
  cout << endl;

  cout << "Question: " << this->question << endl;

  cout << "Answer: " << this->answer << endl;
}

void Descriptive::printAll() {

  for (size_t i = 0; questions[i] != NULL; i++) {

    if (questions[i]->getType() != "descriptive")
      continue;

    cout << endl << "------------------------------------------" << endl;

    cout << "ID: " << i << endl;

    questions[i]->print();

    cout << endl << "------------------------------------------" << endl;
  }
}
