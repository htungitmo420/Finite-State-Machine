#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


using namespace std;

struct Node;
using AST = shared_ptr<Node>;

struct Node {
  string type;
  vector<AST> children;
  string value;

  Node(const string &type, const string &value = "")
      : type(type), value(value) {}
};

class RegexParser {
public:
  RegexParser(const string &regex)
      : tokens(regex.begin(), regex.end()), pos(0) {}

  AST parse() {
    AST result = parse_or();
    if (pos != tokens.size()) {
      throw invalid_argument("Unexpected character at position " +
                             to_string(pos + 1));
    }
    return result;
  }

private:
  vector<char> tokens;
  size_t pos;

  AST parse_or() {
    AST term = parse_concatenation();
    while (pos < tokens.size() && tokens[pos] == '|') {
      ++pos;
      if (pos >= tokens.size()) {
        throw invalid_argument("Missing expression after '|'");
      }
      AST right = parse_concatenation();
      AST node = make_shared<Node>("OR");
      node->children.push_back(term);
      node->children.push_back(right);
      term = node;
    }
    return term;
  }

  AST parse_concatenation() {
    vector<AST> factors;
    while (pos < tokens.size() && tokens[pos] != ')' && tokens[pos] != '|') {
      factors.push_back(parse_factor());
    }
    if (factors.empty()) {
      throw invalid_argument("Empty expression or invalid syntax");
    }
    if (factors.size() == 1)
      return factors[0];

    AST node = make_shared<Node>("CONCAT");
    for (auto &f : factors)
      node->children.push_back(f);
    return node;
  }

  AST parse_factor() {
    AST atom = parse_atom();
    while (pos < tokens.size() && tokens[pos] == '*') {
      ++pos;
      AST node = make_shared<Node>("STAR");
      node->children.push_back(atom);
      atom = node;
    }
    return atom;
  }

  AST parse_atom() {
    if (pos >= tokens.size()) {
      throw invalid_argument("Unexpected end of expression");
    }

    char current = tokens[pos];

    if (current == '(') {
      ++pos;
      if (pos < tokens.size() && tokens[pos] == ')') {
        throw invalid_argument("Empty group '()' is not allowed");
      }
      AST expr = parse_or();
      if (pos >= tokens.size() || tokens[pos] != ')') {
        throw invalid_argument("Missing closing parenthesis ')'");
      }
      ++pos;
      return expr;
    } else if (current == '*' || current == '|' || current == ')') {
      throw invalid_argument(string("Unexpected token '") + current +
                             "' at position " + to_string(pos + 1));
    } else {
      ++pos;
      return make_shared<Node>("CHAR", string(1, current));
    }
  }
};

void print_ast(const AST &ast, int indent = 0) {
  if (!ast)
    return;
  string prefix(indent * 2, ' ');
  cout << prefix << ast->type;
  if (!ast->value.empty())
    cout << "(" << ast->value << ")";
  cout << endl;
  for (const auto &child : ast->children) {
    print_ast(child, indent + 1);
  }
}

int main() {
  string regex;
  getline(cin, regex);

  try {
    RegexParser parser(regex);
    AST ast = parser.parse();
    print_ast(ast);
  } catch (const invalid_argument &e) {
    cerr << "Syntax Error: " << e.what() << endl;
  }

  return 0;
}
