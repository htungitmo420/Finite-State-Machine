#include <iostream>
#include <string>

bool check_two_bs(const std::string &str) {
  enum State { START, A, B1, B2, INVALID };
  State current_state = START;
  int b_count = 0;

  for (char c : str) {
    if (c != 'a' && c != 'b') {
      return false;
    }

    switch (current_state) {
    case START:
    case A:
      if (c == 'a') {
        current_state = A;
      } else if (c == 'b') {
        b_count++;
        if (b_count == 1) {
          current_state = B1;
        } else if (b_count == 2) {
          current_state = B2;
        } else {
          current_state = INVALID;
        }
      }
      break;

    case B1:
    case B2:
      if (c == 'a') {
        current_state = A;
      } else if (c == 'b') {
        b_count++;
        if (b_count > 2) {
          current_state = INVALID;
        } else {
          current_state = B2;
        }
      }
      break;

    case INVALID:
      return false;
    }
  }

  return b_count == 2;
}

int main() {
  std::string input;

  std::cout << "Enter a string: ";
  std::cin >> input;

  bool result = check_two_bs(input);

  std::cout << "Result: " << (result ? "Valid" : "Invalid") << std::endl;
  return 0;
}