#include "auth.h"
#include "date_time.h"
#include "descriptive.h"
#include "four_choice.h"
#include "init.h"
#include "menu_utils.h"
#include "permission.h"
#include "question_base.h"
#include "tag.h"
#include "user.h"

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

    printTitle("Tag Menu");

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
         << "[T] Add a tag to question\n"
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

    case 't': { // Add tag to question
      size_t id;
      cout << "Enter Question ID: ";
      cin >> id;
      cin.ignore();

      if (id > MAX_QUESTIONS - 1 || questions[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find four choice question.");
        break;
      }

      size_t tag_id;
      cout << "Enter Tag ID: ";
      cin >> tag_id;
      cin.ignore();

      if (tag_id > MAX_TAGS - 1 || tags[id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find tag.");
        break;
      }

      questions[id]->addTag(tags[id]);

      WaitForEnterKey();
      break;
    }

    case 'x':
      return;

    default:
      break;
    }
  }
}

void userMenu() {
  while (true) {
    clearScreen();

    cout << "[1] List of users\n"
         << "[2] Create new user\n"
         << "[3] Add permission to a user\n"
         << "[0] Go Back\n";

    char choice = getMenuChoice();
    clearScreen();

    switch (choice) {

    case '1': {
      User::list();
      WaitForEnterKey();
    }

    case '2': {
      string name, username, password;

      cout << "Enter name: ";
      getline(cin, name);

      cout << "Enter username: ";
      getline(cin, username);

      cout << "Enter password: ";
      password = getPassword();

      User *u = User::create(name, username, password, Auth::whoami());
      if (u == NULL) {
        clearScreen();
        printMessagePrompt("You don't have enough permissions.");
        break;
      }

      WaitForEnterKey();
      break;
    }

    case '3': {

      size_t user_id;
      cout << "Enter User ID: ";
      cin >> user_id;
      cin.ignore();

      if (user_id > MAX_USERS - 1 || users[user_id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find selected user.");
        break;
      }

      string title;
      cout << "Enter permission title: ";
      getline(cin, title);

      size_t permission_id = 0;
      for (size_t i = 0; permissions[i] != NULL; i++, permission_id++) {
        if (permissions[i]->getTitle() == title) {
          users[i]->addPermission(permissions[i]);
          break;
        }
      }

      if (permissions[permission_id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find selected permission.");
        break;
      }

      WaitForEnterKey();
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
