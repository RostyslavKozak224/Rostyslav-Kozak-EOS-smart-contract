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

    TABLE question {
        uint64_t id;
        string question_text;
        vector<string> options;
        uint8_t correct_option;

        uint64_t primary_key() const { return id; }
    };

    TABLE student_answer {
        name student;
        uint64_t question_id;
        uint8_t chosen_option;
        uint64_t primary_key() const { return student.value; }
    };

    typedef multi_index<"questions"_n, question> question_table;
    typedef multi_index<"answers"_n, student_answer> answer_table;

    ACTION addquestion(name teacher, uint64_t id, string question_text, vector<string> options, uint8_t correct_option) {
        require_auth(teacher);
        check(question_text.size() > 0, "Вопрос не может быть пустым.");
        check(options.size() >= 2, "Должно быть как минимум 2 варианта ответа.");
        check(correct_option < options.size(), "Неверный индекс правильного ответа.");

        question_table _questions(get_self(), get_self().value);
        auto itr = _questions.find(id);
        check(itr == _questions.end(), "Вопрос с таким ID уже существует.");

        _questions.emplace(get_self(), [&](auto& q) {
            q.id = id;
            q.question_text = question_text;
            q.options = options;
            q.correct_option = correct_option;
        });

        print("Вопрос добавлен: ", question_text);
    }

    ACTION answerq(name student, uint64_t question_id, uint8_t chosen_option) {
        require_auth(student);

        question_table _questions(get_self(), get_self().value);
        answer_table _answers(get_self(), get_self().value);

        auto q_itr = _questions.find(question_id);
        check(q_itr != _questions.end(), "Вопрос не найден.");
        check(chosen_option < q_itr->options.size(), "Выбранный вариант ответа недопустим.");

        auto ans_itr = _answers.find(student.value);
        check(ans_itr == _answers.end(), "Вы уже ответили на этот вопрос.");

        _answers.emplace(get_self(), [&](auto& a) {
            a.student = student;
            a.question_id = question_id;
            a.chosen_option = chosen_option;
        });

        if (chosen_option == q_itr->correct_option) {
            print("Правильный ответ!");
        } else {
            print("Неправильный ответ. Попробуйте снова.");
        }
    }

    ACTION getscore(name student) {
        require_auth(student);

        answer_table _answers(get_self(), get_self().value);
        question_table _questions(get_self(), get_self().value);

        auto ans_itr = _answers.find(student.value);
        check(ans_itr != _answers.end(), "Вы пока не отвечали на вопросы.");

        uint64_t score = 0;
        for (auto& ans : _answers) {
            auto q_itr = _questions.find(ans.question_id);
            if (q_itr != _questions.end() && ans.chosen_option == q_itr->correct_option) {
                score++;
            }
        }

        print("Ваш балл: ", score);
    }
};

