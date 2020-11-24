#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

#define BEFORE_SUM (a.before[i] + b.before[i])
#define AFTER_SUM (a.after[i] + b.after[i])

#define BEFORE_SUB (a.before[i] - b.before[i])
#define AFTER_SUB (a.after[i] - b.after[i])

#define OVERFLOW cout << "Переполнение"
#define UNREAL "Переполнение! Невозможно посчитать результат операции"

#define A_MORE_B "Первое число больше второго"
#define A_LESS_B "Первое число меньше второго"

using namespace std;

const int N = 100;

struct num {
    bool znak;
    int before[N];
    int count_before;
    int after[N];
    int count_after;
};

//считывание
bool read_num(ifstream&, num&);

//запись в файл
void write_num(ofstream&, num);

//+
num summator(num, num);

//-
num sub(num, num);

//равность до точки
bool equal_before(num, num);

//равность после точки
bool equal_after(num, num);

//равны/не равны
string equal(num, num);

//больше/меньше
string more_less(num, num);

//проверка на мусор
bool is_trash(num, int, bool);

//поразрядный перенос
void shift(num*, num, num, int, bool, int*, bool);

int main()
{
    setlocale(LC_ALL, "RUS");
    //num n, k;
    num first_num;//, second_num;

    ifstream in("Input.txt"); // окрываем файл для чтения
    ofstream out("Output.txt"); // открываем файл на запись
    char ch;
    char empty;
    int count = 0;
    num c;
    try {
        //РЕАЛИЗАЦИЯ
        //cout << "НАЧАЛО РАБОТЫ ПРОГРАММЫ" << '\n' << endl;
        if (read_num(in, first_num) == true)
            write_num(out, first_num);
        else throw logic_error("Дальнейшая работа программы невозможна! Исправьте входные данные.");
        do {
            ch = in.get();
            if ((ch != '+') && (ch != '-'))
                throw logic_error("Не встречен знак операции");
            empty = in.get();
            if (empty != '\n')
                throw logic_error("Встречены посторонние символы в строке со знаком");
            num second_num;
            if (read_num(in, second_num) == true)
                write_num(out, second_num);
            else throw logic_error("Дальнейшая работа программы невозможна! Исправьте входные данные.");

            //num c2;

            if (count > 0) {
                if (ch == '+')
                    c = summator(c, second_num);
                else
                    c = sub(c, second_num);
            }
            else {

                if (ch == '+')
                    c = summator(first_num, second_num);
                else
                    c = sub(first_num, second_num);
            }
            //для промежуточных результатов
            write_num(out, c);
            count += 1;
            cout << count << " операция посчитана" << '\n' << endl;
        } while (!in.eof());
        //write_num(out, c);
        //ТЕСТИРОВАНИЕ ФУНКЦИЙ
        /*if (read_num(in, n) == true) {

            write_num(out, n);

        }

        if (read_num(in, k) == true) {

            write_num(out, k);

        }

        num c = summator(n, k);
        //num c = sub(n, k);
        //cout << equal(n, k) << endl;
        //cout << more_less(n, k) << endl;
        out << endl;
        write_num(out, c); */
    }
    catch (logic_error &exception) {
        cout << exception.what() << endl;
        out << "FATAL ERROR" << endl;
    }
    in.close();
    out.close();
    cout << "Программа завершила свою работу :D" << endl;

    return 0;

}

bool is_trash(num c, int i, bool ber_afr) {
    if (ber_afr) {
        if (-abs(c.before[i]) < -800000000)
            return true;
        else return false;
    }
    else
    {
        if (-abs(c.before[i]) < -800000000)
            return true;
        else return false;
    }
}
void shift(num* c, num a, num b, int i, bool bef_afr, int* flag, bool sum_sub) {
    //True - до запятой, False - после
    int first, second, third;
    num cc = *c;
    int main_check;
    if (bef_afr)
        main_check = cc.before[i];
    else main_check = cc.after[i];

    if ((!(is_trash(*c, i, bef_afr))) && (main_check > 7)) {
        if (sum_sub) {
            if (bef_afr == true) {
                first = BEFORE_SUM % 10;
                second = (BEFORE_SUM / 10) % 10;
                third = (BEFORE_SUM / 100);
            }
            else {
                first = AFTER_SUM % 10;
                second = (AFTER_SUM / 10) % 10;
                third = (AFTER_SUM / 100);
            }
        }
        else {
            if (bef_afr == true) {
                first = abs(BEFORE_SUB % 10);
                second = abs((BEFORE_SUB / 10) % 10);
                third = abs((BEFORE_SUB / 100));
            }
            else {
                first = abs(AFTER_SUB % 10);
                second = abs((AFTER_SUB / 10) % 10);
                third = abs((AFTER_SUB / 100));
            }
        }

        while ((first > 7) || (second > 7) || (third > 7)) {
            if (first > 7) {
                first -= 8;
                second += 1;
            }
            if (second > 7) {
                second -= 8;
                third += 1;
            }
            if (third > 7) {
                third -= 8;
                if (((i + 1) <= N))
                    *flag += 1;
            }
        }
        if (bef_afr)
            cc.before[i] = (third * 100) + (second * 10) + first;
        else cc.after[i] = (third * 100) + (second * 10) + first;
        *c = cc;
    }

}
bool read_num(ifstream& in, num& n) {
    string line;
    bool cheack = false;
    bool cheack_number = false;
    int before[30];
    int k = 0;
    int i = 0;
    int line_size;
    int after = 0;
    int count_after = 0;
    n.znak = false;
    int count_string = 0;

    if (in.is_open())
    {   //считывание пока не встречено верно написанное число или конец файла
        while ((cheack_number != true) && !(in.eof()))
        {
            //int before[N];
            getline(in, line);
            if ((line.size() != 0) && (((line[0] >= '0') && (line[0] < '8')) || ((line[1] >= '0') && (line[1] < '8')))) {

                if ((line[0] == '-') && ((line[1] >= '0') && ((line[1] >= '0') && (line[1] < '8')))) {
                    n.znak = true;
                    line.erase(0, 1);
                }
                else if ((line[0] >= '0') && (line[0] < '8'))
                    n.znak = false;
                else {
                    cout << "Ошибка в написании числа:" << endl;
                    cout << line << endl;
                    return false;
                }

                //удаление ведущих нулей
                if (line[0] == '0') {
                    while (line[k] == '0')
                        k += 1;

                    //cout << line << endl;
                    if (line[k + 1] != '.')
                        k -= 1;
                    line.erase(0, k);
                    //cout << line << endl;
                }

                //cout << "Нули в конце:" << endl;

                //удаление нулей в конце
                if ((line[line.size() - 1] == '0') && (((line.size() - line.find('.')) % 3 == 0))){
                    //cout << line.size() << endl;
                    int k = line.size() - 1;
                    while (line[k] == '0')
                        k -= 1;
                    //cout << line << endl;
                    if (line[k + 1] != '.')
                        k += 1;
                    line.erase(k + 1, line.size() - 1);
                    //cout << line << endl;
                }

                int k = 0;
                if (((line[0] >= '0') || (line[0] < '8'))) {
                    cheack = false;
                    //cout << line.size();
                    while ((line[i] != '.') && ((i != (line.size())) && (cheack == false))) {
                        if ((i != line.size()) && ((line[i] >= '0') && (line[i] < '8')) && ((i / 3) < N)) {
                            before[i] = (int)line[i] - 48;
                            i += 1;
                        }
                        else cheack = true;
                    }
                    n.count_before = i;
                }

                if (i == (line.size()))
                    cheack = true;

                if ((line[i] == '.') && (cheack == false) && (i != (line.size())) && ((line[0] >= 0) || (line[0] < 8)))
                {
                    i += 1;
                    int three = 0;
                    int E = 100;
                    while (i != line.size() && (cheack == false)) {
                        if (((i != line.size() + 1) && ((line[0] >= 0) || (line[0] < 8))) && (k <= N))
                        {
                            while ((three < 3) && (i != line.size()) && (cheack != true) && ((i / 3) < N))
                            {
                                if ((line[i] >= '0') && (line[i] < '8')) {
                                    after += E * ((int)line[i] - 48);
                                    three += 1;
                                    i += 1;
                                    E /= 10;
                                    if (i == line.size()) {
                                     //   if (three == 2)
                                    //        after = after / 10;
                                    }
                                }
                                else cheack = true;
                            }
                            n.after[k] = after;
                            after = 0;
                            k += 1;
                            three = 0;
                            E = 100;
                            if (i == line.size())
                                n.count_after = k;
                        }
                        else cheack = true;
                    }
                }
                if (cheack == true) {
                    cout << "Ошибка в написании числа:" << endl;
                    cout << line << endl;
                    return false;
                }
                else
                {
                    cout << "Число записано правильно:" << endl << line << endl;
                    cheack_number = true;
                }
            }
            else
            {
                if (line.size() == 0) {
                    cout << "Строка пуста" << endl;
                    return false;
                }
                else {
                    cout << "Ошибка в написании числа:" << endl;
                    return false;
                }
                cout << line << endl;
            }
            line_size = line.size();

        }

        //if (in.eof())
        //    cout << "Достигнут конец файла\n";

    }
    //cout << boolalpha << n.znak << noboolalpha << ' ';

    //Разделение числа по 3 символа
    int j = n.count_before;
    int count_three = 0;
    int count_before = 0;
    int E = 1;
    int before_three = 0;
    bool overflow = false;
    if (cheack == false) {
        while (j > 0) {
            if (count_before > N) {
                overflow = true;
                break;
            }
            before_three = 0;
            count_three = j - 3;
            if (count_three >= 0) {
                for (int k = 0; k < 3; ++k) {
                    E = 100;
                    for (int i = 0; i < k; i++)
                    {
                        E /= 10;
                    }
                    before_three += E * before[count_three + k];
                    j -= 1;
                }
                n.before[count_before] = before_three;
                count_before += 1;
            }
            else {
                if (count_three == -1) {
                    count_three += 1;
                    E = 1;
                    before_three = 0;
                    for (int k = 0; k < 2; ++k) {
                        E = 10;
                        for (int i = 0; i < k; i++)
                        {
                            E /= 10;
                        }
                        before_three += E * before[count_three + k];
                        j -= 1;
                    }
                    n.before[count_before] = before_three;
                    count_before += 1;
                }

                if (count_three == -2) {
                    n.before[count_before] = before[0];
                    j -= 1;
                    count_before += 1;
                }
            }
        }
    }

    //можно возвращать overflow, чтобы если ошибка в записи, дальше просто не идти
    if (overflow) {
        //cout << "Слишком большое число!" << endl;
        throw logic_error("Слишком большое входное число!");
        return false;
    }

    n.count_before = count_before;

    /*for (int l = 0; l < n.count_before; ++l)
        cout << n.before[l];
    cout << '.';
    for (int l = 0; l < n.count_after; ++l)
        cout << n.after[l];
    cout << endl;*/

    //in.close();
    if (cheack != true)
        return true;
    else return false;
}
void write_num(ofstream& out, num n) {
    if (n.znak == true)
        out << '-';

    for (int i = n.count_before - 1; i >= 0; --i)
    {
        if (i == n.count_before - 1)
            out << n.before[i];
        else {
            if (n.before[i] < 10)
                out << 0 << 0 << n.before[i];
            else if (n.before[i] < 100)
                out << 0 << n.before[i];
            else out << n.before[i];
        }
    }

    out << '.';

    for (int i = 0; i <= n.count_after - 1; ++i) {
        if (i == n.count_after - 1)
        {
            if ((n.after[i] % 100) == 0) {
                out << n.after[i] / 100;
            }
            else if (((n.after[i] % 10) == 0))
                if (n.after[i] == 10)
                    out << '0' << n.after[i] / 10;
                else out << n.after[i] / 10;
            else { 
                if (n.after[i] < 9)
                    out << '0' << '0' << n.after[i];
                else out << n.after[i]; }
        }
        else {
            if (n.after[i] < 100)
                out << '0' << n.after[i];
            else if (n.after[i] < 10)
                out << '0' << '0' << n.after[i];
            else out << n.after[i];
        }
    }

    out << endl;
    //out.close();

}
num summator(num a, num b) {
    num c, swap;
    bool over_flow = false;
    bool check_size = false;

    if (((a.znak == true) && (b.znak == false)) || ((a.znak == false) && (b.znak == true))) {
        if (a.znak) {
            a.znak = false;
            return sub(b, a);
        }
        else if (b.znak) {
            b.znak = false;
            return sub(a, b);
        }

    }

    //поиск числа с наибольшим количеством разрядов до точки
    if (a.count_before < b.count_before)
    {
        swap = a;
        a = b;
        b = swap;
    }

    //инициализация нулями в меньшем числе разрядов до точки
    if (a.count_before > b.count_before) {
        for (int i = b.count_before; i < a.count_before; i++)
        {
            b.before[i] = 0;
        }
    }

    //если знаки разные, сводим к разности
    c.count_before = a.count_before;
    if (a.znak == b.znak)
        c.znak = a.znak;
    else return sub(a, b);

    bool flow = false;
    int i = 0;
    int flag = 0;
    //суммирование разрядов до точки
    while (!(flow) && !(over_flow) && (i < N))
    {

        if ((flag == 0) && (i >= a.count_before)) {
            flow = true;
            continue;
        }

        if (i > c.count_before) {
            c.before[i] = 0 + flag;
            c.count_before += 1;
        }
        else {
            if ((is_trash(a, i, true)) && (is_trash(b, i, true))) {
                a.before[i] = 0;
                b.before[i] = 0;
            }
            c.before[i] = a.before[i] + b.before[i] + flag;
        }
        flag = 0;

        shift(&c, a, b, i, true, &flag, true);
        if ((flag > 0) && (i > c.count_before))
            c.count_before += 1;

        if ((c.count_before > N) && (flag > 0))
        {
            over_flow = true;
            continue;
        }

        ++i;
    }
    num swapp;
    if ((a.count_after < b.count_after) && (over_flow != true))
    {
        swapp = a;
        a = b;
        b = swapp;
    }
    c.count_after = a.count_after;
    if (a.count_after > b.count_after) {
        for (int i = b.count_after; i < a.count_after; i++)
        {
            b.after[i] = 0;
        }
    }
    if (over_flow == true) {
        //OVERFLOW << " до запятой" << endl;
        throw logic_error(UNREAL);
    }

    over_flow = false;
    c.count_after = a.count_after;
    flag = 0;
    flow = false;
    i = c.count_after - 1;

    //суммирование разрядов после точки
    while (!(flow) && !(over_flow) && (i < N))
    {
        if ((flag == 0) && (i < 0)) {
            flow = true;
            continue;
    }

        if ((i < 0) && (flag > 0)) {
            //if ((c.after[0] / 100) > 7) {
                c.before[0] += flag;
                int local_flag = 0;
                num new_b;
                for (int i = 0; i < c.count_before; i++)
                {
                    if (local_flag > 0)
                        c.before[i] += local_flag;
                    local_flag = 0;
                    new_b.before[i] = 0;
                    shift(&c, c, new_b, i, true, &local_flag, true);
                }
                if (local_flag > 0) {
                    if ((c.count_before + 1) <= N) {
                        c.before[c.count_before] = local_flag;
                        c.count_before += 1;
                    }
                    else {
                        over_flow = true;
                        //OVERFLOW << " до запятой" << endl;
                        //return c;
                        throw logic_error(UNREAL);
                    }
                }

            //}
        }
        else
            c.after[i] = a.after[i] + b.after[i] + flag;
        flag = 0;

        shift(&c, a, b, i, false, &flag, true);

        --i;
    }

    if (over_flow == true)
        //OVERFLOW << " после запятой" << endl;
        throw logic_error(UNREAL);

    return c;
}
num sub(num a, num b) {
    num c;
    //случаи, когда разность можно свести к сумме
    if (((a.znak == true) && (b.znak == false)) || ((a.znak == false) && (b.znak == true))) {
        if (b.znak == false)
            b.znak = true;
        else b.znak = false;
        c = summator(a, b);
        if ((a.znak == true) && (b.znak == false))
            c.znak = true;
        return c;
    }

    //инициализация разрядов числа с наименьшим количеством разрядов
    if (a.count_before > b.count_before) {
        for (int i = b.count_before; i < a.count_before; i++)
        {
            b.before[i] = 0;
        }
        c.count_before = a.count_before;
    }
    else {
        if (a.count_before < b.count_before) {
            for (int i = a.count_before; i < b.count_before; i++)
            {
                a.before[i] = 0;
            }
        }
        c.count_before = b.count_before;
    }

    bool over_flow = false;
    bool flow = false;
    int i = 0;
    int flag = 0;
    //разность разрядов до точки
    while (!(flow) && !(over_flow) && (i < N))
    {

        if ((i == N) && (flow > 0))
        {
            over_flow = true;
            continue;
        }
        else {
            if (!((is_trash(a, i, true)) && (is_trash(a, i, true))))
                c.before[i] = abs(a.before[i] - b.before[i] + flag);
            else break;
        }
        flag = 0;

        shift(&c, a, b, i, true, &flag, false);

        if ((flag == 0) && (i >= a.count_before))
            flow = true;
        ++i;
    }

        //c.count_after = a.count_after;
    if (a.count_after > b.count_after) {
        for (int i = b.count_after; i < a.count_after; i++)
        {
            b.after[i] = 0;
        }
    }
    if (over_flow == true)
        //OVERFLOW << "до запятой!" << endl;
        throw logic_error(UNREAL);

    over_flow = false;
    c.count_after = a.count_after;
    flag = 0;
    flow = false;
    i = c.count_after - 1;

    if (((a.znak == false) && (b.znak == false) &&
        ((more_less(a, b)) == A_LESS_B)) || ((a.znak == true) && (b.znak == true) &&
            ((more_less(a, b)) == A_MORE_B)))
        c.znak = true;
    else c.znak = false;

    //разность разрядов после точки
    while (!(flow) && !(over_flow) && (i < N))
    {
        if ((i == N) && (flow > 0))
        {
            over_flow = true;
            continue;
        }

        if (i < 0) {
            if (abs((c.after[0] / 100)) > 7) {
                c.before[0] -= flag;
                int local_flag = 0;
                num new_b;
                for (int i = 0; i < c.count_before; i++)
                {
                    if (local_flag > 0)
                        c.before[i] -= local_flag;
                    local_flag = 0;
                    new_b.before[i] = 0;
                    shift(&c, c, new_b, i, true, &local_flag, true);
                }
                if (local_flag > 0) {
                    if ((c.count_before + 1) <= N) {
                        c.before[c.count_before] = local_flag;
                        c.count_before += 1;
                    }
                    else {
                        over_flow = true;
                        //OVERFLOW << " до запятой" << endl;
                        throw logic_error(UNREAL);
                        //return c;
                    }
                }

            }
        }
        else
            c.after[i] = abs(a.after[i] - b.after[i] + flag);
        flag = 0;
        shift(&c, a, b, i, false, &flag, false);

        if ((flag == 0) && (i < 0))
            flow = true;
        --i;
    }

    if (over_flow == true)
        //OVERFLOW << "после запятой!" << endl;
        throw logic_error(UNREAL);

    return c;
}
bool equal_before(num a, num b) {
    bool EqualBefore = false;
    for (int i = a.count_before - 1; i >= 0; --i)
    {
        if ((a.before[i] == b.before[i]))
            EqualBefore = true;
        else
            return false;
    }
    return true;
}
bool equal_after(num a, num b) {
    bool EqualAfter = false;
    for (int i = 0; i < a.count_after; ++i)
    {
        if ((a.after[i] == b.after[i]))
            EqualAfter = true;
        else
            return false;
    }
    return true;

}
string equal(num a, num b) {
    if ((equal_before(a, b)) && (equal_after(a, b)))
        return "Числа равны";
    else return "Числа не равны";
}
string more_less(num a, num b) {
    bool more_less_before = false;
    bool more_less_after = false;
    if (a.count_before > b.count_before)
        return A_MORE_B;
    else if (a.count_before < b.count_before)
        return A_LESS_B;
    else {
        for (int i = a.count_before - 1; i >= 0; --i)
        {
            if ((more_less_before != true) && (a.before[i] > b.before[i]))
                more_less_before = true;
        }
        if (more_less_before)
            return A_MORE_B;
        else if (equal_before(a, b))
        {
            if (equal_after(a, b) != true) {
                if (a.count_after > b.count_after)
                    return A_MORE_B;
                else if (a.count_after < b.count_after)
                    return A_LESS_B;
                else {
                    for (int i = 0; i < a.count_after; ++i)
                    {
                        if ((more_less_after != true) && (a.after[i] > b.after[i]))
                            more_less_after = true;
                    }

                    if (more_less_after)
                        return A_MORE_B;
                    else return A_LESS_B;
                }
            }
            else
                return "Числа равны";
        }
        else return A_LESS_B;
    }
}