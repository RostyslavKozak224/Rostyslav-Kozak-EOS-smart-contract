#include <eosio/eosio.hpp>
#include <string>

using namespace eosio;
using std::string;

CONTRACT studenttest : public contract {
public:
    using contract::contract;

    studenttest(name receiver, name code, datastream<const char*> ds)
        : contract(receiver, code, ds) {}

    ACTION regstudent(name student, string student_name) {
        require_auth(get_self());
        student_table students(get_self(), get_self().value);
        auto itr = students.find(student.value);
        check(itr == students.end(), "Студент уже зарегистрирован");
        students.emplace(get_self(), [&](auto &s) {
            s.student = student;
            s.student_name = student_name;
            s.score = 0;
        });
    }

    ACTION subscore(name student, uint64_t score) {
        require_auth(student);
        student_table students(get_self(), get_self().value);
        auto itr = students.find(student.value);
        check(itr != students.end(), "Студент не найден");
        students.modify(itr, get_self(), [&](auto &s) {
            s.score = score;
        });
    }

private:
    TABLE student {
        name    student;
        string  student_name;
        uint64_t score;

        uint64_t primary_key() const { return student.value; }
    };

    typedef multi_index<"students"_n, student> student_table;
};

