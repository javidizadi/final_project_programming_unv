#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

void printTitle(string, size_t);
void WaitForEnterKey();
void printTextInBox(string);
char readKeyPress();
char getMenuChoice();
string getPassword();
void clearScreen();

void printTitle(string title, size_t xPadding = 5) {
  printTextInBox(string(xPadding, ' ') + title + string(xPadding, ' '));
}

void WaitForEnterKey() {
  cout << "Press Enter to continue...";
  while (readKeyPress() != '\n')
    ;
}

void printTextInBox(string text) {
  string body = string(text.length() + 2, '=');
  cout << body << endl << '|' << text << '|' << endl << body << endl;
}

#ifdef _WIN32
char readKeyPress() { return _getch(); }
#elif _POSIX_C_SOURCE >= 2 // macro feature testing for popen and pclose
char readKeyPress() {

  fflush(stdout); // write everthing remains in stdout buffer.

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
  cout << endl << "Enter : ";

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
  while ((buf = readKeyPress()) != '\n') {
    password += buf;
    cout << '*';
  }

  cout << endl;

  return password;
}
