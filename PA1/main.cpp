#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <pthread.h>
using namespace std;

struct myStruct
{
    myStruct();
    string s, b;
    double p, f, calc, size;
};

myStruct::myStruct()
{
    s = "", p = 0, calc = 0, f = 0, size = 0, b = "";
}

void* FBar(void* MyPointer);
void* Size(void* MyPointer);
void* Code(void* MyPointer);


/* 1. Arrange the symbols according to decreasing probabilities.
   2. Calculate cumulative probability.
   3. Calculate modified cumulative distribution function.
   4. Find the length of the code.
   5. Generate the code word by finding the binary of Fbar(x) with respect to length l(x).
*/

int main()
{
    int num = 0;
    vector <string> symbols;
    string input = "";
    vector <double> calc;

    while (cin >> input)
    {
        num = 0;
        if (isdigit(input[0]))
            if (input[0] == '0')
            {
                {
                    calc.push_back(stod(input));
                    num += 1;
                    while (cin >> input)
                    {
                        calc.push_back(stod(input));
                        num = num + 1;
                    }
                    break;
                }
            }
        symbols.push_back(input);
        num = num + 1;
    }

    myStruct* x = new myStruct[num];

    double var = 0;

    for (int i = 0; i < num; i++)
    {
        var = var + calc[i];
        x[i].calc = var;
        for(int i = 0; i < num; i++){
            x[i].s = symbols[i];
            for (int i = 0; i < num; i++){
                x[i].p = calc[i];
            }
        }
    }

    pthread_t* T = new pthread_t[num];

    for (int i = 0; i < num; i++)
    {
        pthread_create(&T[i], NULL, FBar, &x[i]);
    }

    for (int i = 0; i < num; i++)
    {
        pthread_join(T[i], NULL);
    }

    for (int i = 0; i < num; i++)
    {
        pthread_create(&T[i], NULL, Size, &x[i]);
        for (int i = 0; i < num; i++){
            pthread_join(T[i], NULL);
        }
    }
    
    for (int i = 0; i < num; i++)
    {
        pthread_create(&T[i], NULL, Code, &x[i]);
        for (int i = 0; i < num; i++){
            pthread_join(T[i], NULL);
        }
    }

    cout << "SHANNON-FANO-ELIAS Codes:\n\n";

    for (int i = 0; i < num; i++)
    {
        cout << "Symbol " << x[i].s << ",";
        cout << " Code: " << x[i].b;

        if (!(i == (num - 1)))
            cout << "\n";
    }

    delete[] x;
    delete[] T;
    return 0;
}

void* FBar(void* MyPointer)
{
    myStruct* pointer = (myStruct*)MyPointer;
    pointer->f = ((pointer->p) * 0.5);
    pointer->f = pointer->f + (pointer->calc - pointer->p);
    return NULL;
}

void* Size(void* MyPointer)
{
    myStruct* pointer = (myStruct*)MyPointer;
    pointer->size = 1 / pointer->p;
    pointer->size = (log2(pointer->size) + 1);
    pointer->size = ceil(pointer->size);
    return NULL;
}

void* Code(void* MyPointer)
{
    myStruct* pointer = (myStruct*)MyPointer;
    double temp = pointer->f;

    for (int i = 0; i < pointer->size; i++)
    {
        temp = temp * 2;
        if (temp >= 1)
        {
            pointer->b = pointer->b + '1';
            temp--;
        }
        else
            pointer->b = pointer->b + '0';
    }
    return NULL;
}
