
#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
const char BACKSPACE_CHAR = 8;
const char ENTER_CHAR = '\r';
#else
#include <cstdio>
const char BACKSPACE_CHAR = 127;
const char ENTER_CHAR = '\n';
#endif

using namespace std;

void printTitle(string, size_t);
void WaitForEnterKey();
void printTextInBox(string);
void printMessagePrompt(string);
char readKeyPress();
char getMenuChoice();
string getPassword();
void clearScreen();

void printTitle(string title, size_t xPadding = 5) {
  printTextInBox(string(xPadding, ' ') + title + string(xPadding, ' '));
}

void WaitForEnterKey() {
  cout << "Press Enter to continue...";
  while (readKeyPress() != ENTER_CHAR)
    ;
}

void printTextInBox(string text) {
  string body = string(text.length() + 6, '=');
  cout << body << endl << "|  " << text << "  |" << endl << body << endl;
}

void printMessagePrompt(string text) {
  printTextInBox(text);
  WaitForEnterKey();
}

#ifdef _WIN32
char readKeyPress() { return _getch(); }
#elif _POSIX_C_SOURCE >= 2 // macro feature testing for popen and pclose
char readKeyPress() {

  fflush(stdout); // write everything remains in stdout buffer.

  char choice;

  FILE *p = popen("read -s -n 1 choice && echo $choice", "r");
  if (p == NULL || (choice = fgetc(p)) == EOF) {
    cout << "Failed to read input." << endl;
    exit(2);
  }
  pclose(p);

  return choice;
}
#endif

char getMenuChoice() {
  cout << endl << "Select Item: ";

  return readKeyPress();
}

#ifdef _WIN32
void clearScreen() { system("cls"); }
#else
void clearScreen() { system("clear"); }
#endif

string getPassword() {
  string password;

  char buf;
  while ((buf = readKeyPress()) != ENTER_CHAR) {

    if (buf == BACKSPACE_CHAR) {

      if (password.length() == 0)
        continue;

      cout << "\b \b";
      password.pop_back();

    } else {
      password += buf;
      cout << '*';
    }
  }
  cout << endl;

  return password;
}
