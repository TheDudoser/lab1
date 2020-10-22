#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

const int N = 10;
const int max_int = 2147483647;

struct num {
    bool znak;
    int before[N];
    int count_before;
    int after[N];
    int count_after;
};

//если в предыдущей строчке встречается символы, то программа выводит не то число
bool read_num(ifstream& in, num& n) {
    string line;
    bool cheack = false;
    bool cheack_number = false;
    int before[N];
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
            if ((line.size() != 0) && (((line[0] >= '0') && (line[0] < '8')) || ((line[1] >= '0') && (line[1] < '8'))))  {

                if ((line[0] == '-') && ((line[1] >= '0') && (line[1] < '8')))
                    n.znak = true;
                else n.znak = false;

                //удаление ведущих нулей
                if (((n.znak == true) && (line[1] == '0')) || ((n.znak == false) && (line[0] == '0'))) {
                    if (n.znak == true)
                        k += 1;
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
                    line.erase(k+1, line.size() - 1);
                    cout << line << endl;
                }

                int k = 0;
                if ((((line[0] >= '0') || (line[0] < '8'))) || ((line[0] == '-') && ((line[1] >= '0') || (line[1] < '8')))) {
                    cheack = false;
                    if (n.znak == true)
                        i = 1;
                    while ((line[i] != '.') && ((i != (line.size())) && (cheack == false))) {
                            if ((i != line.size()) && ((line[0] >= '0') || (line[0] < '8'))) {
                                before[i] = (int)line[i] - 48;
                                i += 1;
                            }
                        else if ((line[i] != '.') || (i-1)>N) cheack = true;
                    }
                    if (n.znak != true)
                        n.count_before = i;
                    else n.count_before = i - 1;
                }

                if (i == (line.size())) 
                    cheack = true;

                if ((line[i] == '.') && (cheack == false) && (i != (line.size())) && ((line[0] >= 0) || (line[0] < 8))) 
                {
                    i += 1;
                    int three = 0;
                    int E = 100;
                    while (i != line.size() && (cheack == false)) {
                        if (((i != line.size() + 1) && ((line[0] >= 0) || (line[0] < 8))) && (k<=N)) 
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
    if (n.znak == true)
        n.count_after = line_size - n.count_before - 2;
    cout << boolalpha << n.znak << noboolalpha << ' ';

    int j = n.count_before;
    int count_three = 0;
    int count_before = 0;
    int E = 1;
    int before_three = 0;
    if (cheack == false) {
        while (j > 0) {
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

    for (int i = n.count_before-1; i >= 0; --i)
            out << n.before[i];

    out << '.';

    for (int i = 0; i <= n.count_after - 1; ++i)
        out << n.after[i];

    //out.close();

}

//+
//пока оч сырое (нужно нормально записывать количество разрядов, 
//учитывать переполнение по разрядам, и учитывать знаки перед числами)
num summator(num a, num b) {
    num c, swap;
    bool over_flow = false;
    //bool before_after; //true - инициализация before, false - after

    if (a.count_before < b.count_before)
    {
        swap = a;
        a = b;
        b = swap;
    }

    if (a.count_before > b.count_before) {
        for (int i = b.count_before; i < a.count_before; i++)
        {
            b.before[i] = 0;
        }
    }
    c.count_before = a.count_before;
        c.znak = a.znak;
        for (int i = 0; i < a.count_before; i++)
        {
            if ((sizeof(a.before[i] + b.before[i]) <= sizeof(int)) && (over_flow != true))
                c.before[i] = a.before[i] + b.before[i];
            else {
                if ((i + 1) <= N)
                    c.before[i + 1] = (a.before[i] + b.before[i]) - max_int;
                    over_flow = true;
            }
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

        for (int i = 0; i < a.count_after; i++)
        {
            if ((sizeof(a.before[i] + b.before[i]) <= sizeof(int)) && (over_flow != true))
                c.after[i] = a.after[i] + b.after[i];
            else
                over_flow = true;
        }

        if (over_flow == true)
            cout << "Переполнение" << endl;

    return c;
}

//-
/*num difference(num a, num b) {
    //добавить учитывание переполнения и подобного
    num c;


    return c;
}*/

//равны/не равны
/*string equal(num a, num b) {


}*/

//больше/меньше
/*string more_less(num a, num b) {

}*/

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

    num c = summator(n, k);
    out << endl;
    write_num(out, c);
    out.close();


    return 0;

}
