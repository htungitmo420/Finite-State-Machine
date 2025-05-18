#include <iostream>
#include <string>

enum State { START, ONE_B, TWO_B, TOO_MANY_B, INVALID, STATE_COUNT };

enum Input { INPUT_A, INPUT_B, INPUT_OTHER, INPUT_COUNT };

State transition_table[STATE_COUNT][INPUT_COUNT];

void initialize_transition_table() {
  transition_table[START][INPUT_A] = START;
  transition_table[START][INPUT_B] = ONE_B;
  transition_table[START][INPUT_OTHER] = INVALID;

  transition_table[ONE_B][INPUT_A] = ONE_B;
  transition_table[ONE_B][INPUT_B] = TWO_B;
  transition_table[ONE_B][INPUT_OTHER] = INVALID;

  transition_table[TWO_B][INPUT_A] = TWO_B;
  transition_table[TWO_B][INPUT_B] = TOO_MANY_B;
  transition_table[TWO_B][INPUT_OTHER] = INVALID;

  transition_table[TOO_MANY_B][INPUT_A] = TOO_MANY_B;
  transition_table[TOO_MANY_B][INPUT_B] = TOO_MANY_B;
  transition_table[TOO_MANY_B][INPUT_OTHER] = INVALID;

  transition_table[INVALID][INPUT_A] = INVALID;
  transition_table[INVALID][INPUT_B] = INVALID;
  transition_table[INVALID][INPUT_OTHER] = INVALID;
}

Input get_input_type(char c) {
  if (c == 'a')
    return INPUT_A;
  if (c == 'b')
    return INPUT_B;
  return INPUT_OTHER;
}

bool check_two_bs_table(const std::string &str) {
  State current_state = START;

  for (char c : str) {
    Input input = get_input_type(c);
    current_state = transition_table[current_state][input];

    if (current_state == INVALID || current_state == TOO_MANY_B) {
      return false;
    }
  }

  return current_state == TWO_B;
}

int main() {
  initialize_transition_table();

  std::string input;
  std::cout << "Enter a string: ";
  std::getline(std::cin, input);

  bool result = check_two_bs_table(input);
  std::cout << "The string " << (result ? "contains" : "does not contain")
            << " exactly two 'b' characters." << std::endl;
  std::cout << std::endl;

  return 0;
}