#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
    if (!assert_true("is_tail('w')", is_tail('w'))) return false; 
    if (!assert_true("is_tail('a')", is_tail('a')))  return false; 
    if(!assert_true("is_tail('s')", is_tail('s'))) return false; 
    if (!assert_true("is_tail('d')", is_tail('d'))) return false; 
    if(!assert_false("is_tail('W')", is_tail('W'))) return false; 
    if (!assert_false("is_tail('x')", is_tail('x'))) return false; 
    if(!assert_false("is_tail('>')", is_tail('>'))) return false; 
    return true;
}

bool test_is_head() {
    if (!assert_true("is_head('W')", is_head('W'))) return false;
    if (!assert_true("is_head('A')", is_head('A'))) return false; 
    if(!assert_true("is_head('S')", is_head('S'))) return false; 
    if (!assert_true("is_head('D')", is_head('D'))) return false; 
    if(!assert_true("is_head('x')", is_head('x'))) return false; 
    if (!assert_false("is_head('w')", is_head('w'))) return false; 
    if(!assert_false("is_head('^')", is_head('^'))) return false; 
    return true;
}

bool test_is_snake() {
    if (!assert_true("is_snake('w')", is_snake('w'))) return false;
    if (!assert_true("is_snake('A')", is_snake('A'))) return false;
    if(!assert_true("is_snake('^')", is_snake('^'))) return false; 
    if (!assert_true("is_snake('<')", is_snake('<'))) return false;
    if(!assert_true("is_snake('>')", is_snake('>'))) return false;
    if (!assert_true("is_snake('x')", is_snake('x'))) return false; 
    if(!assert_false("is_snake('z')", is_snake('z'))) return false;
    if (!assert_false("is_snake('1')", is_snake('1'))) return false; 
    return true;
}

bool test_body_to_tail() {
    if (!assert_true("body_to_tail('^') == 'w'", body_to_tail('^') == 'w')) return false;
    if (!assert_true("body_to_tail('<') == 'a'", body_to_tail('<') == 'a')) return false; 
    if (!assert_true("body_to_tail('v') == 's'", body_to_tail('v') == 's')) return false;
    if(!assert_true("body_to_tail('>') == 'd'", body_to_tail('>') == 'd')) return false; 
    return true;
}

bool test_head_to_body() {
    if (!assert_true("head_to_body('W') == '^'", head_to_body('W') == '^')) return false;
    if (!assert_true("head_to_body('A') == '<'", head_to_body('A') == '<')) return false;
    if (!assert_true("head_to_body('S') == 'v'", head_to_body('S') == 'v')) return false;
    if (!assert_true("head_to_body('D') == '>'", head_to_body('D') == '>')) return false;
    return true;
}

bool test_get_next_row() {
    if (!assert_true("get_next_row(5, 'v') == 6", get_next_row(5, 'v') == 6)) return false; 
    if (!assert_true("get_next_row(5, 's') == 6", get_next_row(5, 's') == 6)) return false; 
    if (!assert_true("get_next_row(5, 'S') == 6", get_next_row(5, 'S') == 6)) return false;
    if (!assert_true("get_next_row(5, '^') == 4", get_next_row(5, '^') == 4)) return false;
    if(!assert_true("get_next_row(5, 'w') == 4", get_next_row(5, 'w') == 4)) return false; 
    if (!assert_true("get_next_row(5, 'W') == 4", get_next_row(5, 'W') == 4)) return false; 
    if (!assert_true("get_next_row(5, '>') == 5", get_next_row(5, '>') == 5)) return false; 
    return true;

}

bool test_get_next_col() {
    if (!assert_true("get_next_col(5, '>') == 6", get_next_col(5, '>') == 6)) return false; 
    if (!assert_true("get_next_col(5, 'd') == 6", get_next_col(5, 'd') == 6)) return false;
    if (!assert_true("get_next_col(5, 'D') == 6", get_next_col(5, 'D') == 6)) return false;
    if (!assert_true("get_next_col(5, '<') == 4", get_next_col(5, '<') == 4)) return false; 
    if (!assert_true("get_next_col(5, 'a') == 4", get_next_col(5, 'a') == 4)) return false;
    if (!assert_true("get_next_col(5, 'A') == 4", get_next_col(5, 'A') == 4)) return false; 
    if (!assert_true("get_next_col(5, 'w') == 5", get_next_col(5, 'w') == 5)) return false; 
    return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}