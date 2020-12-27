#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

#define OVERFLOW cout << "Переполнение"
#define UNREAL "Переполнение! Невозможно посчитать результат операции"

#define A_MORE_B "Первое число больше второго"
#define A_LESS_B "Первое число меньше второго"

using namespace std;

const int N = 10;

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

//равны/не равны
string equal(num, num);

//больше/меньше
string more_less(num, num);

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
    num cc;
    num second_num;
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
            second_num.count_before = 0;
            second_num.count_after = 0;
            c.count_before = 0;
            c.count_after = 0;
            if (read_num(in, second_num) == true)
                write_num(out, second_num);
            else throw logic_error("Дальнейшая работа программы невозможна! Исправьте входные данные.");

            if (count > 0) {
                if (ch == '+')
                    c = summator(cc, second_num);
                else
                    c = sub(cc, second_num);
            }
            else {

                if (ch == '+')
                    c = summator(first_num, second_num);
                else
                    c = sub(first_num, second_num);
                cc = c;
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
    catch (logic_error& exception) {
        cout << exception.what() << endl;
        out << "FATAL ERROR" << endl;
    }
    in.close();
    out.close();
    cout << "Программа завершила свою работу :D" << endl;

    return 0;

}

//Мусор
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

//Разность чисел по 3 разряда
void shift_sub(num* c, num a, num b, int i, bool bef_afr, int* flag) {
    //True - до запятой, False - после
    int first, second;
    num cc = *c;

    if (bef_afr == true) {
        first = (((a.before[i] % 10) - (b.before[i] % 10) - *flag) % 8);
        *flag = (((a.before[i] % 10) - (b.before[i] % 10) - *flag) / 8);
        if (first < 0) {
            first += 8;
            *flag = 1;
        }
        second = (((a.before[i] / 10) % 10) - ((b.before[i] / 10) % 10) - *flag) % 8 * 10 + first;
        *flag = (((a.before[i] / 10) % 10) - ((b.before[i] / 10) % 10) - *flag) / 8;
        if (second < 0) {
            second += 80;
            *flag = 1;
        }
        cc.before[i] = ((a.before[i] / 100 - b.before[i] / 100 - *flag) % 8) * 100 + second;
        *flag = (a.before[i] / 100 - b.before[i] / 100 - *flag) / 8;
        if (cc.before[i] < 0) {
            cc.before[i] += 800;
            *flag = 1;
        }
    }
    else {
        first = (((a.after[i] % 10) - (b.after[i] % 10) - *flag) % 8);
        *flag = (((a.after[i] % 10) - (b.after[i] % 10) - *flag) / 8);
        if (first < 0) {
            first += 8;
            *flag = 1;
        }
        second = (((a.after[i] / 10) % 10) - ((b.after[i] / 10) % 10) - *flag) % 8 * 10 + first;
        *flag = (((a.after[i] / 10) % 10) - ((b.after[i] / 10) % 10) - *flag) / 8;
        if (second < 0) {
            second += 80;
            *flag = 1;
        }
        cc.after[i] = (a.after[i] / 100 - b.after[i] / 100 - *flag) % 8 * 100 + second;
        *flag = (a.after[i] / 100 - b.after[i] / 100 - *flag) / 8;
        if (cc.after[i] < 0) {
            cc.after[i] += 800;
            *flag = 1;
        }
    }

    *c = cc;
    //cout << "--" << i << "-->" << cc.after[i];
}

//Сумма чисел по 3 разряда
void shift(num* c, num a, num b, int i, bool bef_afr, int* flag) {
    //True - до запятой, False - после
    int first, second;
    num cc = *c;

    if (bef_afr == true) {
        first = (((a.before[i] % 10) + (b.before[i] % 10) + *flag) % 8);
        *flag = (((a.before[i] % 10) + (b.before[i] % 10) + *flag) / 8);
        second = (((a.before[i] / 10) % 10) + ((b.before[i] / 10) % 10) + *flag) % 8 * 10 + first;
        *flag = (((a.before[i] / 10) % 10) + ((b.before[i] / 10) % 10) + *flag) / 8;
        cc.before[i] = ((a.before[i] / 100 + b.before[i] / 100 + *flag) % 8) * 100 + second;
        *flag = (a.before[i] / 100 + b.before[i] / 100 + *flag) / 8;
    }
    else {
        first = (((a.after[i] % 10) + (b.after[i] % 10) + *flag) % 8);
        *flag = (((a.after[i] % 10) + (b.after[i] % 10) + *flag) / 8);
        second = (((a.after[i] / 10) % 10) + ((b.after[i] / 10) % 10) + *flag) % 8 * 10 + first;
        *flag = (((a.after[i] / 10) % 10) + ((b.after[i] / 10) % 10) + *flag) / 8;
        cc.after[i] = (a.after[i] / 100 + b.after[i] / 100 + *flag) % 8 * 100 + second;
        *flag = (a.after[i] / 100 + b.after[i] / 100 + *flag) / 8;
    }

    *c = cc;

}

bool read_num(ifstream& in, num& n) {
    string line;
    bool cheack = false;
    bool cheack_number = false;
    int before[3 * N];
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
                if ((line[line.size() - 1] == '0') && (((line.size() - line.find('.')) % 3 == 0))) {
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
                        if ((i != line.size()) && ((line[i] >= '0') && (line[i] < '8')) && (i < 3 * N)) {
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
                            while ((three < 3) && (i != line.size()) && (cheack != true))
                            {
                                if ((line[i] >= '0') && (line[i] < '8')) {
                                    after += E * ((int)line[i] - 48);
                                    three += 1;
                                    i += 1;
                                    E /= 10;
                                    if (i == line.size()) {
                                    }
                                }
                                else cheack = true;
                            }
                            n.after[k] = after;
                            after = 0;
                            k += 1;
                            n.count_after = k;
                            three = 0;
                            E = 100;
                        }
                        else cheack = true;
                    }
                }
                if (k > N)
                    cheack = true;
                if (n.count_after == 0)
                    n.count_after = k;
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
    }

    //Разделение числа по 3 символа
    int j = n.count_before;
    int count_three = 0;
    int count_before = 0;
    int E = 1;
    int before_three = 0;
    bool overflow = false;
    if (cheack == false) {
        while (j > 0) {
            if (count_before < 0) {
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

    if (overflow) {
        throw logic_error("Слишком большое входное число!");
        return false;
    }

    n.count_before = count_before;

    if (cheack != true)
        return true;
    else return false;
}
void write_num(ofstream& out, num n) {
    if (n.znak == true)
        out << '-';

    //удаление лишних нулей до точки
    int l = n.count_before - 1;
    if (n.before[l] == 0) {
        while ((n.before[l] == 0) & (l != 0)) {
            l -= 1;
            n.count_before -= 1;
        }
    }

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

    //удаление лишних нулей после точки
    l = 0;
    if (n.after[l] == 0) {
        while ((n.after[l] == 0) & (l != (l + n.count_after - 1))) {
            l += 1;
            n.count_after -= 1;
        }
    }

    for (int i = 0; i < n.count_after; ++i) {
        if (i == n.count_after - 1)
        {
            if ((n.after[i] % 100) == 0) {
                out << n.after[i] / 100;
            }
            else if (((n.after[i] % 10) == 0))
                if (((n.after[i] / 10) > 0) && ((n.after[i] / 10) < 8))
                    out << '0' << n.after[i] / 10;
                else out << n.after[i] / 10;
            else {
                if (n.after[i] < 9)
                    out << '0' << '0' << n.after[i];
                else if ((n.after[i] / 100) == 0)
                    out << '0' << n.after[i];
                else out << n.after[i];
            }
        }
        else {
            if (n.after[i] < 10)
                out << '0' << '0' << n.after[i];
            else if (n.after[i] < 100)
                out << '0' << n.after[i];
            else out << n.after[i];
        }
    }

    out << endl;
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
        for (int i = b.count_before; i <= a.count_before; i++)
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

        if ((i >= c.count_before) && (flag > 0)) {
            c.before[i] = 0 + flag;
            c.count_before += 1;
        }
        else {
            if ((is_trash(a, i, true)) && (is_trash(b, i, true))) {
                a.before[i] = 0;
                b.before[i] = 0;
            }
            shift(&c, a, b, i, true, &flag);
        }

        shift(&c, a, b, i, true, &flag);
        if ((flag > 0) && (i > c.count_before))
            c.count_before += 1;

        if ((c.count_before >= N) && (flag > 0))
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
        for (int i = b.count_after; i <= a.count_after; i++)
        {
            b.after[i] = 0;
        }
    }
    if (over_flow == true) {
        throw logic_error(UNREAL);
    }

    over_flow = false;
    c.count_after = a.count_after;
    flag = 0;
    flow = false;
    i = c.count_after - 1;

    //суммирование разрядов после точки
    while (!(flow) && !(over_flow) && (i >= -1))
    {
        if ((flag == 0) && (i < 0)) {
            flow = true;
            continue;
        }

        if ((i < 0) && (flag > 0)) {
            c.before[0] += flag;
            int local_flag = 0;
            num new_b;
            for (int i = 0; i < c.count_before; i++)
            {
                if (local_flag > 0)
                    c.before[i] += local_flag;
                local_flag = 0;
                new_b.before[i] = 0;
                shift(&c, c, new_b, i, true, &local_flag);
            }
            if (local_flag > 0) {
                if ((c.count_before + 1) <= N) {
                    c.before[c.count_before] = local_flag;
                    c.count_before += 1;
                }
                else {
                    over_flow = true;
                    throw logic_error(UNREAL);
                }
            }
        }
        else
            shift(&c, a, b, i, false, &flag);

        //shift(&c, a, b, i, false, &flag);

        --i;
    }

    if (over_flow == true)
        throw logic_error(UNREAL);

    return c;
}
void Init_before_after(bool after_before, num& a, num& b, num& c)
{
    if (after_before == true) {
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
    }
    else {
        //инициализация разрядов числа с наименьшим количеством разрядов
        if (a.count_after > b.count_after) {
            for (int i = b.count_after; i < a.count_after; i++)
            {
                b.after[i] = 0;
            }
            c.count_after = a.count_after;
        }
        else {
            if (a.count_after < b.count_after) {
                for (int i = a.count_after; i < b.count_after; i++)
                {
                    a.after[i] = 0;
                }
            }
            c.count_after = b.count_after;
        }
    }
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

    num swap;
    bool check = false;
    bool over_flow = false;
    bool flow = false;
    int i = 0;
    int flag = 0;
    bool be_swap = false;

    if (more_less(a, b) == A_LESS_B) {
        swap = a;
        a = b;
        b = swap;
        be_swap = true;
    }

    Init_before_after(true, a, b, c);

    /*if (a.count_before < b.count_before)
        c.count_before = a.count_before;
    else
        c.count_before = b.count_before; */

        //c.count_after = a.count_before;

    cout << a.before[0] << endl;

    //разность разрядов до точки
    while (!(flow) && (i < N))
    {
        c.before[i] = abs((a.before[i] - flag) - b.before[i]);
        //flag = 0;
        shift_sub(&c, a, b, i, true, &flag);

        for (int k = i + 1; k <= c.count_before; k++)
        {
            if ((a.before[k] > 0) && (b.before[k] > 0) && (flag > 0)) {
                //if (i != c.count_before)
                shift_sub(&c, a, b, k, true, &flag);
                check = true;
            }
        }
        //if (!(check))// && (i != c.count_before))
            //shift_sub(&c, a, b, i, true, &flag);
        check = false;
        
        if (i >= c.count_before)
            flow = true;
        ++i;
    }

    //Init_before_after(true, a, b, c);
    /*int ccount;
    if (a.count_before > b.count_before)
        ccount = a.count_before;
    else ccount = b.count_before

    for (int i = c.count_before; i < ccount; i++)
    {
        Init_before_after(true, a, b, c);
        c.before[i] = a.before[i] + b.before[i];
    } */

    Init_before_after(false, a, b, c);

    flag = 0;
    flow = false;
    i = c.count_after - 1;

    if (be_swap) {
        a.znak = false;
        b.znak = false;
    }

    //инициализация знака c
    if (((a.znak == false) && (b.znak == false) &&
        ((more_less(a, b)) == A_LESS_B)) || ((a.znak == true) && (b.znak == true) &&
            ((more_less(a, b)) == A_MORE_B)))
        c.znak = true;
    else c.znak = false;
    num new_b;
    //разность разрядов после точки
    while (!(flow) && (i < N))
    {

        if (i >= 0)
            shift_sub(&c, a, b, i, false, &flag);

        /*if (flag > 0) {
            for (int k = i - 1; k >= 0; --k)
            {
                if ((a.after[k] >= 0) && (b.after[k] >= 0) && (flag > 0)) {
                    if (abs((a.after[k] % 10) - (b.after[k]) % 10) > 0) {
                        c.after[i] = abs((a.after[i] + 8 * flag) - b.after[i]) + 770;
                        a.after[k] -= 1;
                        new_b.after[i] = 0;
                        shift(&c, c, new_b, i, false, &flag);
                    }
                    else if (abs((a.after[k] / 10) - ((b.after[k] / 10) > 0))) {
                        c.after[i] = abs((a.after[i] + (8 * flag * 10)) - b.after[i]) + 700;
                        a.after[k] -= 10;
                        new_b.after[i] = 0;
                        shift(&c, c, new_b, i, false, &flag);
                    }
                    else if (abs(((a.after[i] / 100) - (b.after[i] / 100)) < 0)) {
                        c.after[i] = abs((a.after[i] + 8 * 100 * flag) - b.after[i]);
                        a.after[k] -= 100;
                        new_b.after[i] = 0;
                        shift(&c, c, new_b, i, false, &flag);
                    }
                    for (int j = k + 1; j < i; j++)
                    {
                        if (a.after[j] == 0)
                            a.after[j] = 777;
                        else if (a.after[j] > 0)
                            a.after[j] += 770;
                        else if ((a.after[j] / 10) > 0)
                            a.after[j] += 700;
                    }
                    check = true;
                }
            }
        } */
        //if (!(check))
        //    c.after[i] = abs(a.after[i] - b.after[i]);
        check = false;
        if ((flag > 0) && ((i - 1) < 0)) {
            shift_sub(&c, a, b, 0, true, &flag);
            //c.before[0] -= flag;
            int local_flag = flag;
            for (int i = 0; i < c.count_before; i++)
            {
                if (local_flag > 0)
                    shift_sub(&c, a, b, i, true, &local_flag);
                    //c.before[i] -= local_flag;
                //local_flag = 0;
                new_b.before[i] = 0;
                //shift_sub(&c, c, new_b, i, true, &local_flag);
                for (int k = i + 1; k <= a.count_before; k++)
                {
                    if ((a.before[i + 1] > 0) && (b.before[i + 1] > 0) && (local_flag > 0)) {
                        shift_sub(&c, a, b, k, true, &local_flag);
                        check = true;
                    }
                }
                check = false;
            }
            if (local_flag > 0) {
                if ((c.count_before + 1) <= N) {
                    c.before[c.count_before] = local_flag;
                    c.count_before += 1;
                }
            }
            flow = true;
            continue;
        }

        //flag = 0;

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
        if (a.before[i] == b.before[i])
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
        if (a.after[i] == b.after[i])
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
