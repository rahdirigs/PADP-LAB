#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

const int WORD_COUNT = 8;

string words[] = {
  "the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog"
};

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: ./gen <num_words>\n";
    exit(0);
  }
  freopen("words.txt", "w", stdout);
  int num_words = atoi(argv[1]);
  srand(time(nullptr));
  for (int i = 0; i < num_words; ++i) {
    int idx = rand() % WORD_COUNT;
    cout << words[idx] << " ";
  }
  return 0;
}

