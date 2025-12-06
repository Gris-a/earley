#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>

class Grammar {
public:
    using set_t = std::unordered_set<char>;
    using mmap_t = std::unordered_multimap<char, std::string>;

    inline static const char start = 0;
private:
    set_t terminal_;
    set_t non_terminal_;
    
    mmap_t rules_;

public:
    Grammar() {
        non_terminal_.emplace(start);
    }

    bool is_terminal(char ch) const noexcept {
        return terminal_.contains(ch);
    }

    bool is_non_terminal(char ch) const noexcept {
        return non_terminal_.contains(ch);
    }

    decltype(auto) rules(char ch) const noexcept {
        return rules_.equal_range(ch);
    }

    friend std::istream &operator>>(std::istream &in, Grammar &grammar) {
        Grammar new_grammar;
        std::string string;

        size_t non_terminal_cnt, terminal_cnt, rules_cnt;
        getline(in, string);
        std::istringstream(string) >> non_terminal_cnt >> terminal_cnt >> rules_cnt;
        
        getline(in, string);
        for (char ch : string) {
            new_grammar.non_terminal_.emplace(ch);
        }

        if (new_grammar.non_terminal_.size() != (non_terminal_cnt + 1)) {
            throw std::exception();
        }

        getline(in, string);    
        for (char ch : string) {
            if (new_grammar.is_non_terminal(ch)) {
                throw std::exception();
            }
            new_grammar.terminal_.emplace(ch);
        }

        if (new_grammar.terminal_.size() != terminal_cnt) {
            throw std::exception();
        }
        
        for (size_t i = 0; i < rules_cnt; ++i) {
            getline(in, string);
            std::istringstream stream(string);

            char rule;
            stream >> rule;

            for (char ch;;) {
                stream >> ch;
                if (ch != '-') continue;
                stream >> ch;
                if (ch == '>') break;
                throw std::exception();
            }

            string = "";
            stream >> string;

            if (!new_grammar.is_non_terminal(rule)) {
                throw std::exception();
            }

            for (char ch : string) {
                if (!new_grammar.is_non_terminal(ch) && !new_grammar.is_terminal(ch)) {
                    throw std::exception();
                }
            }

            new_grammar.rules_.emplace(rule, string);
        }

        
        char start;
        getline(in, string);
        std::istringstream(string) >> start;

        if (!new_grammar.is_non_terminal(start)) {
            throw std::exception();
        }
        new_grammar.rules_.emplace(Grammar::start, std::string(1, start));

        grammar = std::move(new_grammar);
        return in;
    }
};