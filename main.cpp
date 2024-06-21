#include "auth.h"
#include "init.h"
#include "menu_utils.h"

#include <cctype>
#include <iostream>

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
         << "[3] User Menu\n";

    choice = getMenuChoice();

    // TODO
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
    printTextInBox("Incorrect username or password.");
    WaitForEnterKey();
  }
}

// TODO
void tagMenu() {}

// TODO
void questionMenu() {}

// TODO
void userMenu() {}
