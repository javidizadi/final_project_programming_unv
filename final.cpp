// جاوید ایزدی 4026013026
// امیرمهدی امیید علی 4026013019

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

struct DateTime {
  int year, month, day, hour, minutes, seconds;
};

void printDateTime(const DateTime &);
DateTime getCurrentLocalTime();
DateTime getUserDateTime();
bool validateDateTime(const DateTime &);

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

const size_t MAX_TAGS = 100;

class Tag {

private:
  string title;

public:
  Tag(string);
  static Tag *create(string);
  static void printAll();
  void print();
  void addToPointerArray(Tag *[], size_t);
  bool operator==(const Tag &);
};

const size_t MAX_USER_PERMISSIONS = 100;
const size_t MAX_USERS = 100;

class User {
private:
  string name, username, password;
  Permission *permissions[MAX_USER_PERMISSIONS] = {NULL};

public:
  User(string, string, string);
  static User *create(string, string, string, const User *);
  void print();
  bool checkAuth(string, string);
  void addPermission(Permission *);

  string getName();
  string getUsername();
  bool hasPermission(const Permission *) const;
  bool hasPermission(string) const;
  void addToPointerArray(User *[], size_t);
  static void list();
};

class Auth {
private:
  static User *auth;

public:
  static User *login(string, string);
  static void logout();
  static User *whoami();

  static bool authorize(const Permission &);
};

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

  virtual ~Question() {}
};

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

class FourChoice : public Question {

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

void initAdminUser();
// Used in first of main function.
void init() { initAdminUser(); }

#ifdef _WIN32
#include <conio.h>
const char BACKSPACE_CHAR = 8;
const char ENTER_CHAR = '\r';
#else
#include <cstdio>
const char BACKSPACE_CHAR = 127;
const char ENTER_CHAR = '\n';
#endif

void printTitle(string, size_t);
void WaitForEnterKey();
void printTextInBox(string);
void printMessagePrompt(string);
char readKeyPress();
char getMenuChoice();
string getPassword();
void clearScreen();

// ------------------------------- global

User *users[MAX_USERS] = {NULL};

// Hardcoded permissions
Permission ADD_DESCRIPTIVE_QUESTION_PERM("add-descriptive-question");
Permission ADD_FOUR_CHOICE_QUESTION_PERM("add-four-choice-question");
Permission ADD_USER_PERM("add-user");
Permission EDIT_DESCRIPTIVE_QUESTION_PERM("edit-descriptive-question");
Permission EDIT_FOUR_CHOICE_QUESTION_PERM("edit-four-choice-question");

Permission *permissions[MAX_PERMISSIONS] = {
    &ADD_DESCRIPTIVE_QUESTION_PERM, &ADD_FOUR_CHOICE_QUESTION_PERM,
    &ADD_USER_PERM, &EDIT_FOUR_CHOICE_QUESTION_PERM,
    &EDIT_DESCRIPTIVE_QUESTION_PERM};

Question *questions[MAX_QUESTIONS] = {NULL};
Tag *tags[MAX_TAGS] = {NULL};
// ------------------------------- definitions
//
//
User *Auth::auth = NULL;

User *Auth::login(string username, string password) {

  for (size_t i = 0; users[i] != NULL; i++) {

    if (users[i]->checkAuth(username, password)) {
      Auth::auth = users[i];
      return users[i];
    }
  }

  return NULL;
}

void Auth::logout() { Auth::auth = NULL; }

User *Auth::whoami() { return Auth::auth; }

bool Auth::authorize(const Permission &p) {
  User *currentUser = Auth::whoami();

  if (currentUser == NULL) {
    return false;
  }

  return currentUser->hasPermission(&p);
}
void printDateTime(const DateTime &d) {
  cout << d.year << '-';
  cout << setfill('0') << setw(2) << d.month << '-';
  cout << setfill('0') << setw(2) << d.day;

  cout << '\t';

  cout << setfill('0') << setw(2) << d.hour << ':';
  cout << setfill('0') << setw(2) << d.minutes << ':';
  cout << setfill('0') << setw(2) << d.seconds;
}

DateTime getCurrentLocalTime() {
  time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
  tm *time = localtime(&t);

  return DateTime{time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
                  time->tm_hour,        time->tm_min,     time->tm_sec};
}

DateTime getUserDateTime() {

  DateTime result;

  cout << "Enter Year: ";
  cin >> result.year;
  cin.ignore();

  cout << "Enter month: ";
  cin >> result.month;
  cin.ignore();

  cout << "Enter day: ";
  cin >> result.day;
  cin.ignore();

  cout << "Enter hour: ";
  cin >> result.hour;
  cin.ignore();

  cout << "Enter minutes: ";
  cin >> result.minutes;
  cin.ignore();

  cout << "Enter Seconds: ";
  cin >> result.seconds;
  cin.ignore();

  return result;
}

bool validateDateTime(const DateTime &dt) {
  bool isValidDate = (dt.month <= 6 && dt.day <= 31) ||
                     (dt.month > 6 && dt.day <= 30) ||
                     (dt.month == 12 && dt.day <= 29);

  bool isValidTime = (dt.hour <= 24 && dt.minutes <= 60 && dt.seconds <= 60);

  return isValidTime && isValidDate;
}

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

void initAdminUser() {
  User *admin = new User("admin", "admin", "123456");

  for (size_t i = 0; permissions[i] != NULL; i++) {
    admin->addPermission(permissions[i]);
  }

  admin->addToPointerArray(users, MAX_USERS);
}

void freeMemories() {

  for (size_t i = 0; permissions[i] != NULL; i++)
    delete permissions[i];

  for (size_t i = 0; users[i] != NULL; i++)
    delete users[i];

  for (size_t i = 0; tags[i] != NULL; i++)
    delete tags[i];

  for (size_t i = 0; questions[i] != NULL; i++)
    delete questions[i];
}
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

void Tag::addToPointerArray(Tag *arr[], size_t arrSize) {

  // Ensure array isn't full.
  if (arr[arrSize - 1] != NULL)
    return;

  Tag **toWrite = arr;
  while (*toWrite != NULL)
    toWrite++;

  *toWrite = this;
}

Tag::Tag(string t) : title(t) {}

Tag *Tag::create(string title) {
  Tag *tag = new Tag(title);

  for (int i = 0; tags[i] != NULL; i++) {
    if (*tags[i] == *tag) {
      return NULL;
    }
  }

  tag->addToPointerArray(tags, MAX_TAGS);

  return tag;
}

void Tag::print() { cout << title; }

void Tag::printAll() {

  cout << "List of all tags:" << endl;

  for (int i = 0; tags[i] != NULL; i++) {
    cout << '\t';
    cout << '[' << i << ']' << " ";
    tags[i]->print();
    cout << endl;
  }

  cout << endl;
}

bool Tag::operator==(const Tag &t) {

  if (t.title == this->title)
    return true;

  return false;
}

User::User(string n, string u, string p) : name(n), username(u), password(p) {}

User *User::create(string n, string u, string p, const User *currentUser) {

  // Can't use authorize method. Checking manually ( Dependency cycle problem )
  if (currentUser == NULL ||
      currentUser->hasPermission(&ADD_USER_PERM) == false) {
    return NULL;
  }

  User *user = new User(n, u, p);
  user->addToPointerArray(users, MAX_USERS);

  return user;
}

void User::print() {
  cout << "Name: " << this->name << endl
       << "Username: " << this->username << endl;

  cout << "Permissions:" << endl;
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    cout << '\t';
    this->permissions[i]->print();
    cout << endl;
  }
}

bool User::checkAuth(string username, string password) {

  if (this->username == username && this->password == password) {
    return true;
  }

  return false;
}

void User::addPermission(Permission *p) {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (*this->permissions[i] == *p)
      return;
  }
  p->addToPointerArray(this->permissions, MAX_USER_PERMISSIONS);
}

string User::getName() { return this->name; }

bool User::hasPermission(const Permission *p) const {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (*this->permissions[i] == *p)
      return true;
  }
  return false;
}

bool User::hasPermission(string title) const {
  for (size_t i = 0; this->permissions[i] != NULL; i++) {
    if (this->permissions[i]->getTitle() == title)
      return true;
  }
  return false;
}

void User::addToPointerArray(User *arr[], size_t arrSize) {

  if (arr[arrSize - 1] != NULL)
    return;

  int i = 0;
  while (arr[i] != NULL) {
    i++;
  }
  arr[i] = this;
}

string User::getUsername() { return username; }

void User::list() {
  for (size_t i = 0; users[i] != NULL; i++) {
    cout << "------------------------------------" << endl;
    cout << "ID: " << i << endl;
    users[i]->print();
    cout << "------------------------------------" << endl;
  }
}
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
      WaitForEnterKey();
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
      size_t q_id;
      cout << "Enter Question ID: ";
      cin >> q_id;
      cin.ignore();

      if (q_id > MAX_QUESTIONS - 1 || questions[q_id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find four choice question.");
        break;
      }

      size_t tag_id;
      cout << "Enter Tag ID: ";
      cin >> tag_id;
      cin.ignore();

      if (tag_id > MAX_TAGS - 1 || tags[tag_id] == NULL) {
        clearScreen();
        printMessagePrompt("Failed to find tag.");
        break;
      }

      questions[q_id]->addTag(tags[tag_id]);

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

    printTitle("User Menu");

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
      break;
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
          users[user_id]->addPermission(permissions[i]);
          break;
        }
      }

      if (permission_id > MAX_PERMISSIONS - 1 ||
          permissions[permission_id] == NULL) {
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
