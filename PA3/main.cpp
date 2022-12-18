#include <cmath>
#include <vector>
#include <string>
#include <pthread.h>
#include <map>
#include <iostream>

void* MyFunction(void* x);

struct myStruct
{
    int tnum, c = 0;
    double* p;
    char* s;
    pthread_mutex_t* ptr1, * ptr2;
    bool ToF = false;
    pthread_cond_t* ptr3;

    myStruct(double* _p, char* _s, pthread_mutex_t* _ptr1, pthread_mutex_t* _ptr2, pthread_cond_t* _ptr3)
    {
        p = _p, s = _s, ptr1 = _ptr1, ptr2 = _ptr2, ptr3 = _ptr3;
    }

    ~myStruct()
    {
        p, s, ptr1, ptr2 = NULL;
    }
};

int main()
{
    pthread_mutex_t p1;
    pthread_mutex_t p2;
    pthread_mutex_init(&p1, nullptr);
    pthread_mutex_init(&p2, nullptr);
    pthread_cond_t m = PTHREAD_COND_INITIALIZER;
    double sym;
    sym = 0;
    int sym_;
    sym_ = 0;
    std::string y;
    std::getline(std::cin, y);
    std::map <char, int> num;

    for (auto i : y)
    {
        num[i]++;
    }

    for (std::pair<char, int> pair : num)
    {
        sym = sym + pair.second;
        sym_++;
    }

    double* NewPtr;
    NewPtr = new double[sym_];
    double res;
    res = 0;
    char* _s_;
    _s_ = new char[sym_];
    int count;
    count = 0;

    for (std::pair<char, int> pair : num)
    {
        NewPtr[count] = pair.second;
        NewPtr[count] /= sym;
        _s_[count] = pair.first;
        count++;
    }
    myStruct new_(NewPtr, _s_, &p1, &p2, &m);
    pthread_t* tr;
    tr = new pthread_t[sym_];
    int j;

    for (j = 0; j < sym_; j++)
    {
        pthread_mutex_lock(&p1);
        new_.tnum = j;
        pthread_create(&tr[j], NULL, MyFunction, &new_);
    }
    int k;
    for (k = 0; k < sym_; k++)
    {
        pthread_join(tr[k], NULL);
    }
    pthread_mutex_destroy(&p1);
    pthread_mutex_destroy(&p2);
    delete[] tr, _s_, NewPtr;
    return 0;
}

void* MyFunction(void* x)
{
    myStruct* str;
    str = (myStruct*)x;
    int n;
    n = str->tnum;
    double calc;
    calc = str->p[n];
    pthread_mutex_unlock(str->ptr1);
    double res;
    res = 0;
    int y;
    y = 0;

    for (y = 0; y < n + 1; y++)
    {
        res += str->p[y];
    }

    double fb, prod1, prod2;
    fb = 0;
    prod1 = res - calc;
    prod2 = 0.5 * calc;
    fb = prod2 + prod1;
    double s, calc2;
    s = 0;
    calc2 = 1 / calc;
    s = ceil(log2(calc2) + 1);
    float temp = fb;
    int a;
    std::string bin = "";

    for (a = 0; a < s; a++)
    {
        temp *= 2;

        if (temp == 1)
        {
            bin = bin + '1';
            temp--;
        }

        else if (temp > 1)
        {
            bin = bin + '1';
            temp--;
        }

        else
        {
            bin = bin + '0';
        }
    }

    pthread_mutex_lock(str->ptr2);

    if (n != 0)
    {
        while (!(str->ToF))
        {
            pthread_cond_wait(str->ptr3, str->ptr2);
        }

        while (!(n - str->c) == 0)
        {
            pthread_cond_wait(str->ptr3, str->ptr2);
        }
    }
    str->ToF = false;
    pthread_mutex_unlock(str->ptr2);
    if (n == 0)
    {
        std::cout << "SHANNON-FANO-ELIAS Codes:";
        std::cout << "\n\n";
    }
    std::cout << "Symbol " << str->s[n];
    std::cout << ", Code: ";
    std::cout << bin;
    std::cout << "\n";
    pthread_mutex_lock(str->ptr2);
    str->ToF = true;
    str->c++;
    pthread_cond_broadcast(str->ptr3);
    pthread_mutex_unlock(str->ptr2);
    return NULL;
}
