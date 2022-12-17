#include <unistd.h>
#include <sys/wait.h>
#include <map>
#include <pthread.h>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
using namespace std;

void* myFunc2(void* myPtr);

struct myStruct 
{
    string s, bin;
    float calc, res;
    int a1;
    char** a2;
};

int main(int a1, char* a2[])
{
    string userInput;
    std::getline(std::cin, userInput);

    map <char, int> VarSym;

    for (auto i : userInput) 
    {
        VarSym[i] = VarSym[i] + 1;
    }

    double VarSym2 = 0;

    for (auto pair : VarSym)
    {
        VarSym2 = VarSym2 + pair.second;
    }

    int _var = 0;
    int var = VarSym.size();
    myStruct* myList = new myStruct[var];

    for (int x = 0; x < var; x++) 
    {
        myList[x].a1 = a1;
    }

    for (int x = 0; x < var; x++) 
    {
        myList[x].a2 = a2;
    }

    double res_ = 0;

    for (auto pair : VarSym)
    {
        myList[_var].s = pair.first;
        myList[_var].calc = pair.second / VarSym2;
        res_ += myList[_var].calc;
        myList[_var].res = res_;
        _var = _var + 1;
    }

    pthread_t* myList2 = new pthread_t[var]; 

    for (int x = 0; x < var; x++)
    {
        for (int y = 0; y < var; y++) 
        {
            pthread_create(&myList2[x], NULL, myFunc2, &myList[x]);
            pthread_join(myList2[y], NULL);
        }
    }


    std::cout << "SHANNON-FANO-ELIAS Codes:"; 
    std::cout << "\n\n";

    int x;

    for (x = 0; x < var; x++) 
    {
        std::cout << "Symbol " << myList[x].s;
        std::cout << ", Code: " << myList[x].bin;
        std::cout << "\n";
    }

    delete[] myList;
    delete[] myList2;

    return 0;
}

void* myFunc2(void* myPtr)
{
    myStruct* pr = (myStruct*)myPtr;
    int p;
    p = atoi(pr->a2[2]);
    struct sockaddr_in S;
    bzero((char*)&S, sizeof(S));
    S.sin_family = AF_INET;
    struct hostent* srv;
    srv = gethostbyname(pr->a2[1]);
    bcopy((char*)srv->h_addr, (char*)&S.sin_addr.s_addr, srv->h_length);
    S.sin_port = htons(p);
    double w = sizeof(S);
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    auto prep = connect(sk, (struct sockaddr*)&S, w);

    while (prep < 0)
    {
        cerr << "ERROR connecting";
        exit(1);
    } 

    int myCalculation;
    double ar = sizeof(double);
    myCalculation = write(sk, &(pr->calc), ar);
    string prod = "";
    myCalculation = write(sk, &(pr->res), ar);
    int len;
    myCalculation = read(sk, &len, sizeof(int));
    char* co = new char[len + 1];
    bzero(co, len + 1);
    myCalculation = read(sk, co, len);
    close(sk);
    int x;

    for (x = 0; x < len; x++)
    {
        prod = prod + co[x];
    } pr->bin = prod;
    
    delete[] co;
    return NULL;
}
