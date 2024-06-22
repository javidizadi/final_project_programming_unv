#pragma once
#include "auth.h"
#include "date_time.h"
#include "permission.h"
#include "question_base.h"
#include <cctype>
#include <iostream>
using namespace std;

class FourChoice : private Question {

private:
  string A, B, C, D;
  char answer;

public:
  FourChoice(string, DateTime, User, string, string, string, string, char);
  void print();
  static void printAll();

  Question *edit(string, DateTime, User, string);
  Question *edit(string, DateTime, User, string, string, string, string, char);

  static FourChoice *create(string, DateTime, User, string, string, string,
                            string, char);
};

// ---------------------------------
FourChoice::FourChoice(string question, DateTime createdAt, User user, string a,
                       string b, string c, string d, char answer)
    : Question(question, createdAt, user, "four-choice"), A(a), B(b), C(c),
      D(d), answer(toupper(answer)) {}

// Not implemented for descriptive questions (returns NULL)
Question *FourChoice::edit(string q, DateTime createdAt, User u,
                           string answer) {
  return NULL;
}

Question *FourChoice::edit(string q, DateTime createdAt, User user, string a,
                           string b, string c, string d, char answer) {

  if (Auth::authorize(EDIT_FOUR_CHOICE_QUESTION_PERM) == false)
    return NULL;

  this->question = q;
  this->createdAt = createdAt;
  this->user = user;
  this->A = a;
  this->B = b;
  this->C = c;
  this->D = d;
  this->answer = answer;

  return this;
}

FourChoice *FourChoice::create(string question, DateTime createdAt, User user,
                               string a, string b, string c, string d,
                               char answer) {
  if (Auth::authorize(ADD_FOUR_CHOICE_QUESTION_PERM) == false)
    return NULL;

  FourChoice *q = new FourChoice(question, createdAt, user, a, b, c, d, answer);

  q->addToPointerArray(questions, MAX_QUESTIONS);

  return q;
}

void FourChoice::print() {

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

  cout << "A: " << this->A << endl
       << "B: " << this->B << endl
       << "C: " << this->C << endl
       << "D: " << this->D << endl;

  cout << "Answer: " << this->answer << endl;
}

void FourChoice::printAll() {

  for (size_t i = 0; questions[i] != NULL; i++) {

    if (questions[i]->getType() != "four-choice")
      continue;

    cout << endl << "------------------------------------------" << endl;

    cout << "ID: " << i << endl;

    questions[i]->print();

    cout << endl << "------------------------------------------" << endl;
  }
}
