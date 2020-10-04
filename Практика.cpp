#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

const int N = 10;

struct num {
    bool znak;
    int before[N];
    int after[N];
};


bool read_num(ifstream& in, num& n) {
    string line;
    bool cheack = false;
    int before[N];
    int k = 0;
    int i = 0;

    if (in.is_open())
    {
        while (getline(in, line))
        {
            if (line.size() != 0) {

                if ((line[0] == '-') && ((line[1] >= 0) || (line[1] < 8)))
                    n.znak = true;
                else n.znak = false;

                if ((((line[0] >= 0) || (line[0] < 8))) || ((line[0] == '-') && ((line[1] >= 0) || (line[1] < 8)))) {
                    cheack = false;
                    if (n.znak == true)
                        i = 1;
                    while ((line[i] != '.') && ((i != (line.size())) && (cheack == false))) {
                            if ((i != line.size()) && ((line[0] >= 0) || (line[0] < 8))) {
                                before[i] = (int)line[i] - 48;
                                i += 1;
                            }
                        else if ((line[i] != '.') || (i-1)>N) cheack = true;
                    }
                }
                if (i == (line.size())) { cheack = true; }
                if ((line[i] == '.') && (cheack == false) && (i != (line.size())) && ((line[0] >= 0) || (line[0] < 8))) {
                    i += 1;
                    
                    while (i != line.size() && (cheack == false)) {
                        if (((i != line.size() + 1) && ((line[0] >= 0) || (line[0] < 8))) && (k<=N)) {
                            n.after[k] = (int)line[i] - 48;
                            k += 1;
                            i += 1;
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
                }
            }
            else
            {
                cout << "Строка пуста" << endl;
            }
        }
        if (in.bad())
            cout << "Ошибка ввода-вывода при чтении\n";
        else if (in.eof())
            cout << "Достигнут конец файла\n";
        else if (in.fail())
            cout << "Неверный формат данных\n";
    }

    cout << boolalpha << n.znak << noboolalpha << ' ';

    int m;
    if (n.znak != true)
        m = i - k - 1;
    else m = i - k - 2;

    int count_three = 0;
    int count_before = 0;
    int j = m-1;
    if (cheack == false) {
        while (j >= 0) {
            count_three = j - 2;
            if (count_three >= 0) {
                for (int k = 0; k < 3; ++k) {
                    n.before[count_before] = before[count_three + k];
                    count_before += 1;
                    j -= 1;
                }
            }
            else {
                if (count_three == -1) {
                    count_three += 2;
                    for (int k = 0; k <= count_three; ++k) {
                        n.before[count_before] = before[k];
                        count_before += 1;
                        j -= 1;
                    }
                }

                if (count_three == -2) {
                    n.before[count_before] = before[0];
                    j -= 1;
                }
            }
            }
    }

    for (int l = 0; l < m; ++l)
        cout << n.before[l];
    cout << ' ';
    for (int l = 0; l < k; ++l)
        cout << n.after[l];
    cout << endl;

    in.close();
    if (cheack != true)
        return true;
    else return false;
}

void write_num(ofstream& out, num n) {
    //if (n.znak == true)
      //  out << '-';
    /*cout << endl << (sizeof(n.before) / sizeof(n.before[0])) <<endl;
    for (int i = 0; i < (sizeof(n.before) / sizeof(n.before[0])); ++i) {
            cout << n.before[i];
            cout << endl << i;
    }
    //out << n.before << '.' << n.after << endl; */
    
    //out << n << endl;

}

long double summator(long double a, long double b) {
    //добавить учитывание переполнения и подобного
    return a + b;
}

long double difference(long double a, long double b) {
    //добавить учитывание переполнения и подобного
    return a - b;
}

string equal(long double a, long double b) {
    if (a == b)
        return "Числа равны";
    else
        return "Числа не равны";

}

string more_less(long double a, long double b) {
    if (a > b)
        return "Первое число больше второго";
    else if (a < b)
        return "Первое число меньше второго";
    else
        return "Попробуй проверить равны ли числа";
}

int main()
{
    setlocale(LC_ALL, "RUS");
    num n;

    ifstream in("Input.txt"); // окрываем файл для чтения
    ofstream out("Output.txt"); // открываем файл на запись
    
    if (read_num(in, n) == true) {

        //write_num(out, n);

    }
    int a;
    cin >> a;
    return 0;
    
}
