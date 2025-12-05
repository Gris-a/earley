#include <earley.hpp>

int main() {
  Grammar grammar;
  std::cin >> grammar;

  Earley earley;
  earley.fit(std::move(grammar));

  size_t m;
  std::string word;

  getline(std::cin, word);
  std::istringstream(word) >> m;

  for (size_t i = 0; i < m; ++i) {
    getline(std::cin, word);
    std::cout << (earley.predict(word) ? "YES\n" : "NO\n");
  }
}