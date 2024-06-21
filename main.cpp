#include "auth.h"
#include "date_time.h"
#include "descriptive.h"
#include "four_choice.h"
#include "init.h"
#include "menu_utils.h"
#include "question_base.h"

#include <cctype>
#include <iostream>
#include <string>

using namespace std;

void mainMenu();
void loginMenu();

void questionMenu();
void tagMenu();
void userMenu();

int main() {

  init();

  while (true) {
    mainMenu();
  }

  freeMemories();
  return 0;
}

void mainMenu() {
  clearScreen();

  printTitle("Main Menu");

  char choice;

  if (Auth::whoami() == NULL) {
    cout << "[A] Login\n"
         << "[B] View Questions\n"
         << "[X] Exit\n";

    choice = getMenuChoice();

    switch (tolower(choice)) {
    case 'a':
      loginMenu();
      return;

    case 'b':
      clearScreen();
      Question::list();
      return;

    case 'x':
      clearScreen();
      exit(0);

    default:
      return;
    }
  } else {

    cout << "Hello " << Auth::whoami()->getName() << "!" << endl << endl;

    cout << "[1] Question Menu\n"
         << "[2] Tag Menu\n"
         << "[3] User Menu\n"
         << "[4] Logout\n";

    choice = getMenuChoice();

    switch (tolower(choice)) {
    case '1':
      questionMenu();
      return;

    case '2':
      tagMenu();
      return;

    case '3':
      userMenu();
      return;

    case '4':
      Auth::logout();
      return;

    default:
      return;
    }
  }
}

void loginMenu() {
  clearScreen();

  printTitle("Login");

  string username;
  cout << "Username: ";
  getline(cin, username);

  cout << "Password: ";
  string password = getPassword();

  if (Auth::login(username, password) == NULL) {
    clearScreen();
    printMessagePrompt("Incorrect username or password.");
  }
}

void tagMenu() {
  while (true) {
    clearScreen();

    cout << "[1] List of tags with ID\n"
         << "[2] Create Tag\n"
         << "[0] Go Back\n";

    char choice = getMenuChoice();

    clearScreen();
    switch (choice) {

    case '1': {
      Tag::printAll();
      WaitForEnterKey();
      break;
    }

    case '2': {
      string title;
      cout << "Enter title: ";
      getline(cin, title);

      Tag *t = Tag::create(title);

      break;
    }

    case '0': {
      return;
    }

    default:
      break;
    }
  }
}

// TODO
void questionMenu() {
  while (true) {
    clearScreen();

    printTitle("Question Menu");

    cout << "[A] List of Descriptive Questions with ID\n"
         << "[B] Create Descriptive Question\n"
         << "[C] Edit Descriptive Question\n"
         << "[D] Print Descriptive Question\n"
         << "[E] Create Four Choice Question\n"
         << "[F] Edit Four ChoiceQuestion\n"
         << "[G] List of Four Choice Questions with ID\n"
         << "[H] Print Four Choice Question\n"
         << "[I] Publish a Question\n"
         << "[J] Unpublish a Question\n"
         << "[X] Go Back\n";

    char choice = getMenuChoice();

    clearScreen();

    switch (tolower(choice)) {
    case 'a': { // List of Descriptive Questions with ID
      Descriptive::list();
      WaitForEnterKey();
      break;
    }

    case 'b': { // Create Descriptive Question

      string question, answer;

      cout << "Enter question: ";
      getline(cin, question);

      cout << "Enter answer: ";
      getline(cin, answer);

      Descriptive *q =
          Descriptive::create(question, getCurrentLocalTime(), *Auth::whoami());

      if (q == NULL) {
        clearScreen();
        printMessagePrompt("You don't have enough permissions.");
        break;
      }

      q->addAnswer(answer);

      WaitForEnterKey();
      break;
    }

    case 'c': { // Edit Descriptive Question

      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find descriptive question.");
        break;
      }

      if (questions[id]->getType() != "descriptive") {
        clearScreen();
        printMessagePrompt("Selected question isn't descriptive");
        break;
      }

      questions[id]->print();
      cout << endl;

      string question, answer;

      cout << "Enter new question: ";
      getline(cin, question);

      cout << "Enter new answer: ";
      getline(cin, answer);

      Question *q = questions[id]->edit(question, getUserDateTime(),
                                        *Auth::whoami(), answer);
      if (q == NULL) {
        clearScreen();
        printMessagePrompt("You don't have enough permissions.");
      }

      WaitForEnterKey();
      break;
    }

    case 'd': { // Print descriptive question

      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find descriptive question.");
        break;
      }

      if (questions[id]->getType() != "descriptive") {
        clearScreen();
        printMessagePrompt("Selected question isn't descriptive");
        break;
      }

      questions[id]->print();
      WaitForEnterKey();

      break;
    }

    case 'e': { // Create Four Choice Question

      string question, A, B, C, D;
      char answer;

      cout << "Enter question: ";
      getline(cin, question);

      cout << "Enter A text: ";
      getline(cin, A);

      cout << "Enter B text: ";
      getline(cin, B);

      cout << "Enter C text: ";
      getline(cin, C);

      cout << "Enter D text: ";
      getline(cin, D);

      cout << "Enter answer: ";
      answer = toupper(readKeyPress());
      cout << answer << endl;

      FourChoice *f = FourChoice::create(question, getCurrentLocalTime(),
                                         *Auth::whoami(), A, B, C, D, answer);

      if (f == NULL) {
        clearScreen();
        printMessagePrompt("You don't have enough permissions.");
        break;
      }

      WaitForEnterKey();
      break;
    }

    case 'f': { // Edit Four Choice Question

      string question, A, B, C, D;
      char answer;

      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find four choice question.");
        break;
      }

      if (questions[id]->getType() != "four-choice") {
        clearScreen();
        printMessagePrompt("Selected question isn't four choice.");
        break;
      }

      cout << "Enter question: ";
      getline(cin, question);

      cout << "Enter A text: ";
      getline(cin, A);

      cout << "Enter B text: ";
      getline(cin, B);

      cout << "Enter C text: ";
      getline(cin, C);

      cout << "Enter D text: ";
      getline(cin, D);

      cout << "Enter answer: ";
      answer = toupper(readKeyPress());
      cout << answer << endl;

      Question *q = questions[id]->edit(question, getUserDateTime(),
                                        *Auth::whoami(), A, B, C, D, answer);

      if (q == NULL) {
        clearScreen();
        printMessagePrompt("You don't have enough permissions.");
        break;
      }

      WaitForEnterKey();
      break;
    }

    case 'g': { // List of Four Choice Questions with ID
      FourChoice::printAll();
      WaitForEnterKey();
      break;
    }

    case 'h': { // Print Four Choice Question

      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find four choice question.");
        break;
      }

      if (questions[id]->getType() != "four-choice") {
        clearScreen();
        printMessagePrompt("Selected question isn't four choice.");
        break;
      }

      questions[id]->print();
      WaitForEnterKey();

      break;
    }

    case 'i': { // Publish a Question
      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find selected question.");
        break;
      }

      if (questions[id]->isAuthorUser(*Auth::whoami()) == false) {
        clearScreen();
        printMessagePrompt("You aren't author of this question.");
        break;
      }

      questions[id]->publish();

      break;
    }

    case 'j': { // Unpublish a Question
      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find selected question.");
        break;
      }

      if (questions[id]->isAuthorUser(*Auth::whoami()) == false) {
        clearScreen();
        printMessagePrompt("You aren't author of this question.");
        break;
      }

      questions[id]->unPublish();

      break;
    }

    case 'x':
      return;

    default:
      break;
    }
  }
}

// TODO
void userMenu() {}
