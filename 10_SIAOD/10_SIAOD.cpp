#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <regex>

using namespace std;

void record_frequency(const std::string&);
std::string to_lower(const std::string&);
void count_chosen_word(const std::string&);

void print_all_matches(const std::string&, const std::regex&);

/*
Алгоритм: Рабина-Карпа

1)	Проверка на плагиат. Даны два текста, разбитые на слова. Из исходного текста выбирается слово и проверяется, входит ли слово в проверяемый текст. Сформировать список частоты появления слова в текст.
2)	Дан текст. Сформировать список всех email- адреса, содержащихся в этом тексте.

*/

// 1)
std::map<std::string, unsigned> frequency;

void record_frequency(const std::string& str)
{
    if (frequency.count(str))
    {
        frequency[str] += 1;
    }
    else
    {
        frequency[str] = 1;
    }
}

void choose_word()
{
    std::ifstream f("10-1.txt");

    std::set<std::string> set;
    std::string word;

    while (std::getline(f, word, ' '))
    {
        set.insert(to_lower(word)); record_frequency(word);
    }

    std::vector<std::pair<std::string, unsigned>> vec(frequency.begin(), frequency.end());

    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string, unsigned>& a1, const std::pair<std::string, unsigned>& a2)
        { return a1.second > a2.second; });

    std::cout << "Введите какое-либо из следующих слов: " << std::endl;
    ;
    for (auto it = set.crbegin(); it != set.crend(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::string(10, '-') << std::endl;
    
    std::string choice; std::cin >> choice;

    if (!set.сontains(choice))
    {
        throw std::runtime_error("no existing word provided");
    }

    count_chosen_word(choice);
}

/* Наивный алгоритм поиска строки сравнивает заданный шаблон со всеми позициями в тексте. Это приводит к далеко не идеальной сложности времени исполнения O(nm) */
/* Вычисляется хеш шаблона строки.
Вычисляется хеш подстроки в тексте строки, начиная с индекса 0 и до m-
1.
Сравнивается хеш подстроки текста с хешем шаблона.
Если они совпадают, то сравниваются отдельные символы для
выявления точного совпадения двух строк.
Если они не совпадают, то окно подстроки сдвигается путём увеличения индекса и повторяется третий пункт для вычисления хеша следующих m символов, пока не будут пройдены все n символов. */

/* В алгоритме используется скользящий хеш - функция вычисления хеша, позволяющая алгоритму Рабина-Карпа достичь заявленной сложности O(n+m)
(в то время как при использовании плохиш хеш-функций асимптотическая сложность может упасть до O(mn), а то и вовсе быть намного хуже
*/

int get_hash(const std::string& s)
{
    const int p = 31, m = 1e9 + 7; int hash = 0;
    long p_pow = 1;
    for (int i = 0; i < s.length(); i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m; p_pow = (p_pow * p) % m;
    }

    return hash;
}

/* Алгоритм хорош для поиска многих вхождений, но заметно хуже альтернатив для поиска единственного вхождения. */
void count_chosen_word(const std::string& str)
{
    size_t length = str.length();
    long long str_hash = get_hash(str);

    unsigned occurencies = 0; std::ifstream f("10-2.txt");

    std::string text;
    while (std::getline(f, text)) // одна строка
    {
        // хеш первой подстроки длины m:
        long long text_hash = get_hash(text.substr(0, length));

        for (unsigned int k = 0; k <= (text.length() - length); k++)
        {
            if (str_hash == text_hash)
            {
                // если хеши совпали, нужно проверить посимвольно:
                for (unsigned long long i = 0; (i < length) && (str[i] == text[k + i]); i++)
                {
                    // дошли до последнего символа

                    if (i == (length - 1))
                    {
                        // // если нужно проверить, что мы нашли слово, а не вхождение типа string-searching, string-specific и пр.:
                        // if (((text.size() > k + i + 1) && text[k + i + 1] == ' ') || (text.size() == k+i + 1))
                        occurencies++;
                    }
                }
            }
            // хеш следующей подстроки
            text_hash = get_hash(text.substr(k + 1, length));
            // text_hash = ring_hash(text.substr(k), length, text_hash);
        }
    }

    std::cout << occurencies << std::endl;
}

std::string to_lower(const std::string& str)
{
    std::string res;

    for (auto it = str.cbegin(); it != str.cend(); ++it) res += char(tolower(*it));

    return res;
}

// 2)
void print_all_matches(const std::string& text, const std::regex& rx)
{
    std::string::const_iterator text_it = text.cbegin(); std::match_results<std::string::const_iterator> match_results;

    while (std::regex_search(text_it, text.end(), match_results, rx))
    {
        int count = match_results.size();

        std::string resultant_str = std::string(match_results[0].first, match_results[0].second);

        if (resultant_str[0] == ' ')
        {
            resultant_str = resultant_str.substr(1);

        }

        std::cout << resultant_str << std::endl;

        text_it = match_results[0].second;
    }
}

int main()
{
    // 1
    // ввести ''
    choose_word(); // проверить на слове 'string'/'pattern'

    /*
    // 2)
    std::string str = "sadfsadf@ya.ru \ asdfsadfsadfasdfasdffoo.bar_baz123@yandex.ru \ email&with*incorrect#characters@gmail.com \

    too_long_email	_
        _@rambler
    .ru \
        another100	email@yahoo.com \ foo@bar.baz \
    elona@musk.spacex";

    std::string regex_text = "(^[a-zA-Z0-9\\.\\-_]{1,64}@[a-zA-Z0-9\\.\\-
    ]{2,239}\\.[a-zA-Z]{2,16})|(\\s[a-zA-Z0-9\\.\\-_]{1,64}@[a-zA-Z0-9\\.\\-
    ]{2,239}\\.[a-zA-Z]{2,16})";
    std::regex rx(regex_text, std::regex_constants::ECMAScript | std::regex_constants::icase | std::regex_constants::multiline);

    print_all_matches(str, rx);
    */

    return 0;
}
