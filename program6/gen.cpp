#include <iostream>
#include <cstring>
#include <ctime>

#define WORD_COUNT 8

using namespace std;

string words[] = {
  "the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog"
};

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: ./gen <word_count>\n";
    exit(0);
  }
  freopen("words.txt", "w", stdout);
  int word_count = atoi(argv[1]);
  srand(time(nullptr));
  for (int i = 0; i < word_count; ++i) {
    int idx = rand() % WORD_COUNT;
    cout << words[idx] << " ";
  }
  return 0;
}

