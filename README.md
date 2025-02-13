Смарт-контракт для тестування знань студентів на EOS
Опис проєкту
Цей проєкт є смарт-контрактом на мові C++ для блокчейну EOS, який використовується для тестування знань студентів.
Система дозволяє викладачам додавати запитання з варіантами відповідей, а студентам – відповідати на них і отримувати свої результати.

Функціональність
Викладач може:

Додавати запитання у тест
Вказувати правильний варіант відповіді
Студент може:

Обирати варіант відповіді на запитання
Переглядати свій поточний бал
Встановлення та запуск
Встановлення EOSIO
Перед запуском необхідно встановити EOSIO. Переконайтеся, що у вас встановлено:

cleos – CLI-інструмент для роботи з блокчейном EOS
nodeos – вузол мережі EOS
eosio.cdt – інструменти для компіляції смарт-контрактів

Якщо EOSIO не встановлено, виконайте команду:
sudo apt install eosio

Компіляція смарт-контракту

Перейдіть у каталог із контрактом і виконайте команду
eosio-cpp -o student_quiz.wasm quiz.cpp --abigen

Деплой контракту в EOS
cleos set contract <ACCOUNT_NAME> /path/to/contract -p <ACCOUNT_NAME>@active

Де:

<ACCOUNT_NAME> – ваш EOS-акаунт
/path/to/contract – шлях до директорії з student_quiz.wasm
Структура контракту
addquestion(teacher, id, text, options, correct_option) – додає нове запитання
answerq(student, question_id, chosen_option) – студент відповідає на запитання
getscore(student) – отримує поточний бал студента

Приклад роботи

Додавання запитання
cleos push action <ACCOUNT_NAME> addquestion '["teacher", 1, "Скільки буде 2+2?", ["1", "2", "4"], 2]' -p teacher@active

Відповідь студентаt
cleos push action <ACCOUNT_NAME> answerq '["student", 1, 2]' -p student@active

Отримання балу
cleos push action <ACCOUNT_NAME> getscore '["student"]' -p student@active
