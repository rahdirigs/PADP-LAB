#include <cstdio>
#include <cstring>
#include <iostream>
#include <omp.h>

using namespace std;

const char *FILE_NAME = "words.txt";
const int WORD_SIZE = 10;
const int WORD_COUNT = 8;

char words[][WORD_SIZE] = {
  "the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog"
};

int counts[WORD_COUNT];

bool is_alpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

void read_word(char *word, FILE *fp) {
  int idx = 0;
  char ch;
  while ((ch = fgetc(fp)) != EOF && !is_alpha(ch));
  while (ch != EOF && is_alpha(ch)) {
    word[idx] = ch;
    ++idx;
    ch = fgetc(fp);
  }
  word[idx] = '\0';
}

bool is_equal(char *word, const char *key, int ignore_case) {
  int word_len = strlen(word);
  char key_copy[WORD_SIZE];
  strcpy(key_copy, key);
  int key_len = strlen(key);
  if (word_len != key_len) {
    return false;
  }
  if (ignore_case) {
    for (int i = 0; i < word_len; ++i) {
      word[i] = tolower(word[i]);
      key_copy[i] = tolower(key_copy[i]);
    }
  }
  return (strcmp(word, key_copy) == 0);
}

int get_word_count(const char *key, int ignore_case) {
  int count = 0;
  char word[WORD_SIZE];
  FILE *fp = fopen(FILE_NAME, "r");
  while (!feof(fp)) {
    read_word(word, fp);
    if (is_equal(word, key, ignore_case)) {
      ++count;
    }
  }
  return count;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./6 <ignore_case> <num_threads>\n");
    exit(0);
  }
  int ignore_case = atoi(argv[1]);
  int num_threads = atoi(argv[2]);
  int i;
  double start_time = omp_get_wtime();
#pragma omp parallel for private(i) shared(counts, words) num_threads(num_threads)
  for (i = 0; i < WORD_COUNT; ++i) {
    counts[i] = get_word_count(words[i], ignore_case);
  }
  double end_time = omp_get_wtime();
  for (i = 0; i < WORD_COUNT; ++i) {
    printf("Count of word \"%s\" is %d\n", words[i], counts[i]);
  }
  printf("Time taken = %lf\n", end_time - start_time);
  return 0;
}

