#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <vector>

using namespace eosio;
using std::string;
using std::vector;

CONTRACT quiz : public contract {
public:
    using contract::contract;

    quiz(name receiver, name code, datastream<const char*> ds)
        : contract(receiver, code, ds) {}

    ACTION addquestion(name admin, uint64_t id, string question, vector<string> options, uint8_t correct_option) {

        question_index questions(get_self(), get_self().value);
        auto itr = questions.find(id);
        check(itr == questions.end(), "Question ID already exists");

        questions.emplace(admin, [&](auto& row) {
            row.id = id;
            row.question = question;
            row.options = options;
            row.correct_option = correct_option;
        });

        print("Question added: ", question);
    }

    ACTION answerq(name student, uint64_t id, uint8_t answer) {

        question_index questions(get_self(), get_self().value);
        auto itr = questions.find(id);
        check(itr != questions.end(), "Question not found");

        student_index students(get_self(), get_self().value);
        auto student_itr = students.find(student.value);

        if (itr->correct_option == answer) {
            print("Correct answer!");

            if (student_itr == students.end()) {
                students.emplace(student, [&](auto& row) {
                    row.username = student;
                    row.score = 1;
                });
            } else {
                students.modify(student_itr, same_payer, [&](auto& row) {
                    row.score += 1;
                });
            }
        } else {
            print("Wrong answer!");
        }
    }

    ACTION getscore(name student) {

        student_index students(get_self(), get_self().value);
        auto itr = students.find(student.value);

        if (itr == students.end()) {
            print("Student not found.");
        } else {
            print("Score: ", itr->score);
        }
    }

private:
    TABLE question {
        uint64_t id;
        string question;
        vector<string> options;
        uint8_t correct_option;

        uint64_t primary_key() const { return id; }
    };

    TABLE student {
        name username;
        uint64_t score;

        uint64_t primary_key() const { return username.value; }
    };

    using question_index = multi_index<"questions"_n, question>;
    using student_index = multi_index<"students"_n, student>;
};

