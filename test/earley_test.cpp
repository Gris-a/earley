#include <gtest/gtest.h>

#include <earley.hpp>
#include <sstream>
#include <string>

class EarleyParserTest : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(EarleyParserTest, AnBn) {
  std::stringstream grammar_input;
  grammar_input << "1 2 2\n";
  grammar_input << "S\n";
  grammar_input << "ab\n";
  grammar_input << "S->aSb\n";
  grammar_input << "S->\n";
  grammar_input << "S";

  Grammar grammar;
  grammar_input >> grammar;

  Earley earley;
  earley.fit(grammar);

  EXPECT_TRUE(earley.predict(""));
  EXPECT_TRUE(earley.predict("ab"));
  EXPECT_TRUE(earley.predict("aabb"));
  EXPECT_TRUE(earley.predict("aaabbb"));
  EXPECT_TRUE(earley.predict("aaaabbbb"));
  EXPECT_TRUE(earley.predict("aaaaabbbbb"));

  // Отрицательные тесты
  EXPECT_FALSE(earley.predict("a"));
  EXPECT_FALSE(earley.predict("b"));
  EXPECT_FALSE(earley.predict("ba"));
  EXPECT_FALSE(earley.predict("aba"));
  EXPECT_FALSE(earley.predict("abb"));
  EXPECT_FALSE(earley.predict("aab"));
  EXPECT_FALSE(earley.predict("aaabb"));
  EXPECT_FALSE(earley.predict("aabbb"));
  EXPECT_FALSE(earley.predict("abab"));
  EXPECT_FALSE(earley.predict("baab"));
  EXPECT_FALSE(earley.predict("aaabbbbb"));   // Несоответствие количества
  EXPECT_FALSE(earley.predict("aaaaabbbb"));  // Несоответствие количества
  EXPECT_FALSE(earley.predict("c"));          // Посторонний символ
  EXPECT_FALSE(earley.predict("abc"));
  EXPECT_FALSE(earley.predict("aabba"));
}

TEST_F(EarleyParserTest, RightSequence) {
  std::stringstream grammar_input;
  grammar_input << "1 2 2\n";
  grammar_input << "S\n";
  grammar_input << "ab\n";
  grammar_input << "S->aSbS\n";
  grammar_input << "S->\n";
  grammar_input << "S";

  Grammar grammar;
  grammar_input >> grammar;

  Earley earley;
  earley.fit(grammar);

  EXPECT_TRUE(earley.predict(""));
  EXPECT_TRUE(earley.predict("ab"));
  EXPECT_TRUE(earley.predict("abab"));
  EXPECT_TRUE(earley.predict("aabbab"));
  EXPECT_TRUE(earley.predict("abaabaabbb"));
  EXPECT_TRUE(earley.predict("ababab"));
  EXPECT_TRUE(earley.predict("aabb"));
  EXPECT_TRUE(earley.predict("aaabbb"));
  EXPECT_TRUE(earley.predict("abababab"));
  EXPECT_TRUE(earley.predict("aababb"));

  // Отрицательные тесты
  EXPECT_FALSE(earley.predict("a"));
  EXPECT_FALSE(earley.predict("b"));
  EXPECT_FALSE(earley.predict("ba"));
  EXPECT_FALSE(earley.predict("aba"));
  EXPECT_FALSE(earley.predict("bab"));
  EXPECT_FALSE(earley.predict("aa"));
  EXPECT_FALSE(earley.predict("bb"));
  EXPECT_FALSE(earley.predict("abba"));
  EXPECT_FALSE(earley.predict("baab"));
  EXPECT_FALSE(earley.predict("ababa"));
  EXPECT_FALSE(earley.predict("aabba"));
  EXPECT_FALSE(earley.predict("abbab"));
  EXPECT_FALSE(earley.predict("c"));
  EXPECT_FALSE(earley.predict("abc"));
  EXPECT_FALSE(earley.predict("ababc"));
}

TEST_F(EarleyParserTest, ABStarGrammar) {
  std::stringstream grammar_input;
  grammar_input << "1 2 5\n";
  grammar_input << "S\n";
  grammar_input << "ab\n";
  grammar_input << "S->aSa\n";
  grammar_input << "S->a\n";
  grammar_input << "S->b\n";
  grammar_input << "S->bSb\n";
  grammar_input << "S->\n";
  grammar_input << "S";

  Grammar grammar;
  grammar_input >> grammar;

  Earley earley;
  earley.fit(grammar);

  EXPECT_TRUE(earley.predict(""));
  EXPECT_TRUE(earley.predict("a"));
  EXPECT_TRUE(earley.predict("b"));
  EXPECT_TRUE(earley.predict("aa"));
  EXPECT_TRUE(earley.predict("bb"));
  EXPECT_TRUE(earley.predict("aba"));
  EXPECT_TRUE(earley.predict("bab"));
  EXPECT_TRUE(earley.predict("ababaaaaaababa"));
  EXPECT_TRUE(earley.predict("abaaba"));
  EXPECT_TRUE(earley.predict("aabbaa"));
  EXPECT_TRUE(earley.predict("baaab"));
  EXPECT_TRUE(earley.predict("abba"));
  EXPECT_TRUE(earley.predict("ababa"));
  EXPECT_TRUE(earley.predict("babbab"));

  // Отрицательные тесты
  EXPECT_FALSE(earley.predict("c"));
  EXPECT_FALSE(earley.predict("ac"));
  EXPECT_FALSE(earley.predict("ca"));
  EXPECT_FALSE(earley.predict("abc"));
  EXPECT_FALSE(earley.predict("aab"));
  EXPECT_FALSE(earley.predict("bba"));
  EXPECT_FALSE(earley.predict("abca"));
  EXPECT_FALSE(earley.predict("aaba"));
  EXPECT_FALSE(earley.predict("abababaaba"));
}

// Тест 7: Грамматика с цикличными правилами
TEST_F(EarleyParserTest, CyclicGrammar) {
  std::stringstream grammar_input;
  grammar_input << "2 1 4\n";
  grammar_input << "AB\n";
  grammar_input << "a\n";
  grammar_input << "A->aB\n";
  grammar_input << "A->\n";
  grammar_input << "B->aA\n";
  grammar_input << "B->\n";
  grammar_input << "A";

  Grammar grammar;
  grammar_input >> grammar;

  Earley earley;
  earley.fit(grammar);

  // Четная длина
  EXPECT_TRUE(earley.predict(""));
  EXPECT_TRUE(earley.predict("aa"));
  EXPECT_TRUE(earley.predict("aaaa"));
  EXPECT_TRUE(earley.predict("aaaaaa"));
  EXPECT_TRUE(earley.predict("aaaaaaaa"));

  // Нечетная длина
  EXPECT_TRUE(earley.predict("a"));
  EXPECT_TRUE(earley.predict("aaa"));
  EXPECT_TRUE(earley.predict("aaaaa"));
  EXPECT_TRUE(earley.predict("aaaaaaa"));
  EXPECT_TRUE(earley.predict("aaaaaaaaa"));

  // С другими символами
  EXPECT_FALSE(earley.predict("b"));
  EXPECT_FALSE(earley.predict("ab"));
  EXPECT_FALSE(earley.predict("ba"));
  EXPECT_FALSE(earley.predict("aab"));
  EXPECT_FALSE(earley.predict("aba"));
  EXPECT_FALSE(earley.predict("aaab"));
  EXPECT_FALSE(earley.predict("aaba"));
  EXPECT_FALSE(earley.predict("abaa"));
  EXPECT_FALSE(earley.predict("abc"));
  EXPECT_FALSE(earley.predict("aac"));
  EXPECT_FALSE(earley.predict("caa"));
}

TEST_F(EarleyParserTest, EdgeCases) {
  {
    std::stringstream grammar_input;
    grammar_input << "1 0 0\n";
    grammar_input << "S\n";
    grammar_input << "\n";
    grammar_input << "\n";
    grammar_input << "S";

    Grammar grammar;
    grammar_input >> grammar;

    Earley earley;
    earley.fit(grammar);

    EXPECT_FALSE(earley.predict("a"));
    EXPECT_FALSE(earley.predict("b"));
    EXPECT_FALSE(earley.predict("c"));
    EXPECT_FALSE(earley.predict("abc"));
    EXPECT_FALSE(earley.predict("1"));
    EXPECT_FALSE(earley.predict(""));
  }

  {
    std::stringstream grammar_input;
    grammar_input << "1 0 1\n";
    grammar_input << "S\n";
    grammar_input << "\n";
    grammar_input << "S->\n";
    grammar_input << "S";

    Grammar grammar;
    grammar_input >> grammar;

    Earley earley;
    earley.fit(grammar);

    EXPECT_TRUE(earley.predict(""));
    EXPECT_FALSE(earley.predict("a"));
    EXPECT_FALSE(earley.predict("b"));
    EXPECT_FALSE(earley.predict("c"));
    EXPECT_FALSE(earley.predict("abc"));
    EXPECT_FALSE(earley.predict("1"));
    EXPECT_FALSE(earley.predict(" "));
  }

  {
    std::stringstream grammar_input;
    grammar_input << "1 1 1\n";
    grammar_input << "S\n";
    grammar_input << "a\n";
    grammar_input << "S->a\n";
    grammar_input << "S";

    Grammar grammar;
    grammar_input >> grammar;

    Earley earley;
    earley.fit(grammar);

    EXPECT_TRUE(earley.predict("a"));
    EXPECT_FALSE(earley.predict(""));
    EXPECT_FALSE(earley.predict("aa"));
    EXPECT_FALSE(earley.predict("b"));
    EXPECT_FALSE(earley.predict("ab"));
    EXPECT_FALSE(earley.predict("ba"));
    EXPECT_FALSE(earley.predict("aaa"));
    EXPECT_FALSE(earley.predict("a "));
    EXPECT_FALSE(earley.predict(" a"));
  }

  {
    std::stringstream grammar_input;
    grammar_input << "3 3 3\n";
    grammar_input << "SAB\n";
    grammar_input << "abc\n";
    grammar_input << "S->aA\n";
    grammar_input << "A->bB\n";
    grammar_input << "B->c\n";
    grammar_input << "S";

    Grammar grammar;
    grammar_input >> grammar;

    Earley earley;
    earley.fit(grammar);

    EXPECT_TRUE(earley.predict("abc"));
    EXPECT_FALSE(earley.predict("ab"));
    EXPECT_FALSE(earley.predict("bc"));
    EXPECT_FALSE(earley.predict("a"));
    EXPECT_FALSE(earley.predict("b"));
    EXPECT_FALSE(earley.predict("c"));
    EXPECT_FALSE(earley.predict("ac"));
    EXPECT_FALSE(earley.predict("bac"));
    EXPECT_FALSE(earley.predict("abca"));
    EXPECT_FALSE(earley.predict("aabc"));
    EXPECT_FALSE(earley.predict("abcc"));
    EXPECT_FALSE(earley.predict("abcd"));
    EXPECT_FALSE(earley.predict(""));
    EXPECT_FALSE(earley.predict("abcabc"));
    EXPECT_FALSE(earley.predict("abcb"));
  }
}

TEST_F(EarleyParserTest, SameTerminalsGrammar) {
  std::stringstream grammar_input;
  grammar_input << "2 1 3\n";
  grammar_input << "SA\n";
  grammar_input << "a\n";
  grammar_input << "S->aA\n";
  grammar_input << "A->aS\n";
  grammar_input << "A->\n";
  grammar_input << "S";

  Grammar grammar;
  grammar_input >> grammar;

  Earley earley;
  earley.fit(grammar);

  EXPECT_TRUE(earley.predict("a"));
  EXPECT_TRUE(earley.predict("aaa"));
  EXPECT_TRUE(earley.predict("aaaaa"));
  EXPECT_TRUE(earley.predict("aaaaaaa"));
  EXPECT_TRUE(earley.predict("aaaaaaaaa"));

  // Отрицательные тесты - четное количество 'a' или пустая строка
  EXPECT_FALSE(earley.predict(""));
  EXPECT_FALSE(earley.predict("aa"));
  EXPECT_FALSE(earley.predict("aaaa"));
  EXPECT_FALSE(earley.predict("aaaaaa"));
  EXPECT_FALSE(earley.predict("aaaaaaaa"));

  // С другими символами
  EXPECT_FALSE(earley.predict("b"));
  EXPECT_FALSE(earley.predict("ab"));
  EXPECT_FALSE(earley.predict("ba"));
  EXPECT_FALSE(earley.predict("aab"));
  EXPECT_FALSE(earley.predict("aba"));
  EXPECT_FALSE(earley.predict("aaab"));
  EXPECT_FALSE(earley.predict("abc"));
  EXPECT_FALSE(earley.predict("a a"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}