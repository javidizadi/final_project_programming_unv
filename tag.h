#pragma once
#include <iostream>

using namespace std;

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

// ---------------------------------
Tag *tags[MAX_TAGS] = {NULL};

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
