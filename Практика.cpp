#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

const int N = 10;
const int max_num = 777;

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
string more_less(num a, num b);

int main()
{
    setlocale(LC_ALL, "RUS");
    num n, k;

    ifstream in("Input.txt"); // окрываем файл для чтения
    ofstream out("Output.txt"); // открываем файл на запись
    
    if (read_num(in, n) == true) {

        write_num(out, n);

    }
    //in.open("Input.txt");
    if (read_num(in, k) == true) {

        write_num(out, k);

    }
    //out.close();

    //num c = summator(n, k);
    //беды с разностью
    num c = sub(n, k);
    //cout << equal(n, k) << endl;
    //cout << more_less(n, k) << endl;
    out << endl;
    write_num(out, c);
    out.close();


    return 0;

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

                if ((line[0] == '-') && ((line[1] >= '0') && (line[1] < '8'))) {
                    n.znak = true;
                    line.erase(0, 1);
                }
                else n.znak = false;

                //удаление ведущих нулей
                if (((n.znak == true) && (line[1] == '0')) || ((n.znak == false) && (line[0] == '0'))) {
                    while (line[k] == '0')
                        k += 1;

                    cout << line << endl;
                    if (line[k + 1] != '.')
                        k -= 1;
                    line.erase(0, k);
                    cout << line << endl;
                }

                cout << "Нули в конце:" << endl;

                //удаление нулей в конце
                if (line[line.size() - 1] == '0') {
                    cout << line.size() << endl;
                    int k = line.size() - 1;
                    while (line[k] == '0')
                        k -= 1;
                    cout << line << endl;
                    if (line[k + 1] != '.')
                        k += 1;
                    line.erase(k + 1, line.size() - 1);
                    cout << line << endl;
                }

                int k = 0;
                if ((((line[0] >= '0') || (line[0] < '8'))) || ((line[0] == '-') && ((line[1] >= '0') || (line[1] < '8')))) {
                    cheack = false;
                    cout << line.size();
                    while ((line[i] != '.') && ((i != (line.size())) && (cheack == false))) {
                        if ((i != line.size()) && ((line[0] >= '0') || (line[0] < '8'))) {
                            before[i] = (int)line[i] - 48;
                            i += 1;
                        }
                        else if ((line[i] != '.') || (i - 1) > N) cheack = true;
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
                            while ((three < 3) && (i != line.size()))
                            {
                                after += E * ((int)line[i] - 48);
                                three += 1;
                                i += 1;
                                E /= 10;
                            }
                            n.after[k] = after;
                            after = 0;
                            k += 1;
                            three = 0;
                            E = 1;
                            if (i == line.size())
                                n.count_after = k;
                        }
                        else cheack = true;
                    }
                }
                if (cheack == true) {
                    cout << "Ошибка в написании числа:" << endl;
                    cout << line << endl;
                }
                else
                {
                    cout << "Число записано правильно:" << endl << line << endl;
                    cheack_number = true;
                }
            }
            else
            {
                if (line.size() == 0)
                    cout << "Строка пуста" << endl;
                else {
                    cout << "Ошибка в написании числа:" << endl;
                    cheack = true;
                }
                cout << line << endl;
            }
            line_size = line.size();

        }

        if (in.eof())
            cout << "Достигнут конец файла\n";

    }
    cout << boolalpha << n.znak << noboolalpha << ' ';

    int j = n.count_before;
    int count_three = 0;
    int count_before = 0;
    int E = 1;
    int before_three = 0;
    if (cheack == false) {
        while (j > 0) {
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
                }
            }
        }
    }

    n.count_before = count_before;

    for (int l = 0; l < n.count_before; ++l)
        cout << n.before[l];
    cout << ' ';
    for (int l = 0; l < n.count_after; ++l)
        cout << n.after[l];
    cout << endl;

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
            if ((n.after[i] % 100) == 0)
                out << n.after[i] / 100;
            else if ((n.after[i] % 10) == 0)
                out << n.after[i] / 10;
            else out << n.after[i];
        }
        else
            out << n.after[i];
    }

    out << endl;
    //out.close();

}
num summator(num a, num b) {
    num c, swap;
    bool over_flow = false;
    bool check_size = false;

    //if (check_sum_sub(a, b))
        //return sub(a, b);

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

    c.count_before = a.count_before;
    if (a.znak == b.znak)
        c.znak = a.znak;
    else return sub(a, b);

    bool flow = false;
    int i = 0;
    int flag = 0;
    //суммирование разрядов до точки
    while (!(flow) && !(over_flow) && (i <= N))
    {
        if ((i == N) && ((a.before[i] + b.before[i]) > max_num))
        {
            over_flow = true;
            continue;
        }

        if (i > c.count_before) {
            c.before[i] = 0 + flag;
            c.count_before += 1;
        }
        else
            c.before[i] = a.before[i] + b.before[i] + flag;
        flag = 0;
        while (c.before[i] > max_num) {
            c.before[i] -= max_num + 1;
            flag += 1;
        }

        if ((flag == 0) && (i >= a.count_before))
            flow = true;

        //}
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
    if (over_flow == true)
        cout << "Переполнение до запятой!" << endl;

    over_flow = false;
    c.count_after = a.count_after;
    flag = 0;
    flow = false;
    i = 0;

    //суммирование разрядов после точки
    while (!(flow) && !(over_flow) && (i <= N))
    {
        if ((i == N) && ((a.after[i] + b.after[i]) > max_num))
        {
            over_flow = true;
            continue;
        }

        if (i > c.count_after) {
            c.after[i] = 0 + flag;
            c.count_after += 1;
        }
        else
            c.after[i] = a.after[i] + b.after[i] + flag;
        flag = 0;
        while (c.after[i] > max_num) {
            c.after[i] -= max_num + 1;
            flag += 1;
        }

        if ((flag == 0) && (i >= a.count_after))
            flow = true;

        //}
        ++i;
    }

    if (over_flow == true)
        cout << "Переполнение после запятой!" << endl;

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
    while (!(flow) && !(over_flow) && (i <= N))
    {
        if ((i == N) && ((a.before[i] - b.before[i]) < (-1 * max_num)))
        {
            over_flow = true;
            continue;
        }

        if (i > c.count_before) {
            c.before[i] = 0 + flag;
            c.count_before += 1;
        }
        else
            c.before[i] = abs(a.before[i] - b.before[i] + flag);
        flag = 0;
        while (c.before[i] < -(max_num)) {
            c.before[i] -= -(max_num + 1);
            flag += 1;
        }

        if ((flag == 0) && (i >= a.count_before))
            flow = true;

        //}
        ++i;
    }

        c.count_after = a.count_after;
    if (a.count_after > b.count_after) {
        for (int i = b.count_after; i < a.count_after; i++)
        {
            b.after[i] = 0;
        }
    }
    if (over_flow == true)
        cout << "Переполнение до запятой!" << endl;

    over_flow = false;
    c.count_after = a.count_after;
    flag = 0;
    flow = false;
    i = 0;

    if ((a.znak == false) && (b.znak == false) &&
        ((more_less(a, b)) == "Первое число меньше второго"))
        c.znak = true;

    //разность разрядов после точки
    while (!(flow) && !(over_flow) && (i <= N))
    {
        if ((i == N) && ((a.after[i] - b.after[i]) < -max_num))
        {
            over_flow = true;
            continue;
        }

        if (i > c.count_after) {
            c.after[i] = 0 + flag;
            c.count_after += 1;
        }
        else
            c.after[i] = abs(a.after[i] - b.after[i] + flag);
        flag = 0;
        while (c.after[i] < -max_num) {
            c.after[i] += max_num + 1;
            flag += 1;
        }

        if ((flag == 0) && (i >= a.count_after))
            flow = true;

        //}
        ++i;
    }
    if ((-(a.after[0] - b.after[0]) < 0) && ((a.znak == b.znak) == true))
        c.before[0] += 1;

    if (over_flow == true)
        cout << "Переполнение после запятой!" << endl;

    if ((more_less(a, b) == "Первое число больше второго") && (a.znak == b.znak))
        c.znak = true;

    return c;
}
bool equal_before(num a, num b) {
    bool EqualBefore = false;
    for (int i = a.count_before - 1; i >= 0; --i)
    {
        //i = a.count_before;
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
        //i = a.count_after;
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
    else return "Числа различны";
}
string more_less(num a, num b) {
    bool more_less_before = false;
    bool more_less_after = false;
    string a_more_b = "Первое число больше второго";
    string a_less_b = "Первое число меньше второго";
    if (a.count_before > b.count_before)
        return a_more_b;
    else if (a.count_before < b.count_before)
        return a_less_b;
    else {
        for (int i = a.count_before - 1; i >= 0; --i)
        {
            if ((more_less_before != true) && (a.before[i] > b.before[i]))
                more_less_before = true;
        }
        if (more_less_before)
            return a_more_b;
        else if (equal_before(a, b))
        {
            if (equal_after(a, b) != true) {
                if (a.count_after > b.count_after)
                    return a_more_b;
                else if (a.count_after < b.count_after)
                    return a_less_b;
                else {
                    for (int i = 0; i < a.count_after; ++i)
                    {
                        if ((more_less_after != true) && (a.after[i] > b.after[i]))
                            more_less_after = true;
                    }

                    if (more_less_after)
                        return a_more_b;
                    else return a_less_b;
                }
            }
            else
                return "Числа равны";
        }
        else return a_less_b;
    }
}
