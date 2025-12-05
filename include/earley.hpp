#pragma once

#include <grammar.hpp>
#include <vector>
#include <utility>

class Earley {
public:
    struct Situation {
    public:
        using rule_t = Grammar::mmap_t::const_iterator;

    // private:
        rule_t rule_;
        size_t offset_;
        size_t begin_, end_;

    public:
        Situation(rule_t rule, size_t offset, size_t begin, size_t end);

        char rule() const noexcept;
        char anchor() const noexcept;

        size_t begin() const noexcept;

        Situation scan() const noexcept;
        Situation pred(rule_t rule) const noexcept;
        Situation comp(const Situation &situation) const noexcept;

        size_t hash() const noexcept;

        friend bool operator==(const Situation &lhs, const Situation &rhs) = default;
    };

    struct SituationHash {
        size_t operator()(const Situation &situation) const noexcept;
    };

    using set_t = std::unordered_set<Situation, SituationHash>;
    using mmap_t = std::unordered_multimap<char, Situation>;
private:
    std::pair<mmap_t::const_iterator, mmap_t::const_iterator> comp_situations(const Situation &situation) const noexcept;
    std::pair<Grammar::mmap_t::const_iterator, Grammar::mmap_t::const_iterator> pred_situations(const Situation &situation) const noexcept;

    set_t comp(size_t iter) const;
    set_t pred(size_t iter) const;

    void scan(size_t iter, char c) const;
    void closure(size_t iter) const;

    bool predict_prologue(const std::string &word) const;
    bool predict_epilogue(const std::string &word) const;

    Grammar grammar_;

    mutable std::vector<mmap_t> anchored_situations_;
    mutable set_t difference_;

public:
    Earley() = default;

    void fit(Grammar grammar);
    bool predict(const std::string &word) const;
};