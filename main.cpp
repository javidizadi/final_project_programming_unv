#include "auth.h"
#include "init.h"
#include "menu_utils.h"

#include <cctype>
#include <iostream>

using namespace std;

void mainMenu();
void loginMenu();

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

  printMessage("Main Menu");

  int choice;

  if (Auth::whoami() == NULL) {
    cout << "[A] Login\n"
         << "[B] View Questions\n"
         << "[X] Exit\n";

    char choice = getMenuChoice();

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
    cin >> choice;
    cin.ignore();
  }
}

void loginMenu() {
  clearScreen();

  printMessage("Login");

  string username;
  cout << "Username: ";
  getline(cin, username);

  string password;
  cout << "Password: ";
  getline(cin, password);

  if (Auth::login(username, password) == NULL) {
    clearScreen();
    printMessage("Incorrect username or password.");
    // waitToPressEnter();
  }
}
