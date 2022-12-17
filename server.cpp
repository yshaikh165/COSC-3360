#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cmath>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

void myFunc(int) 
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

string BinaryFunction(double calc, double res) 
{
    string bin = "";
    double vv;
    vv = res - calc;
    double v;
    v = 0.5 * calc;
    v += vv;
    double z;
    z = 1 / calc;
    int len = ceil(log2(z) + 1);
    int i;

    for (i = 0; i < len; i++)
    {
        v *= 2;
        if (!(v > 1 || v == 1))
        {
            bin = bin + '0';
        }
        else
        {
            bin = bin + '1';
            v--;
        }
    }

    return bin;
}


int main(int argc, char* argv[])
{
    signal(SIGCHLD, myFunc);
    int sk;
    sk = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sAdd;
    double z = sizeof(sAdd);
    bzero((char*)&sAdd, z);
    int p = atoi(argv[1]);

    sAdd.sin_family = AF_INET;
    sAdd.sin_addr.s_addr = INADDR_ANY;
    sAdd.sin_port = htons(p);

    double y = sizeof(sAdd);
    if (bind(sk, (struct sockaddr*)&sAdd, y) < 0) {
        cerr << "ERROR on binding";
        exit(1);
    }

    listen(sk, 5);
    struct sockaddr_in cAdd;
    int c;
    c = sizeof(cAdd);

    do
    {
        int nf;
        nf = accept(sk, (struct sockaddr*)&cAdd, (socklen_t*)&c);
        if (fork() == 0)
        {
            int MyCalculations;
            float calcx;
            MyCalculations = read(nf, &calcx, sizeof(float));
            float resx;
            MyCalculations = read(nf, &resx, sizeof(float));
            string key;
            key = BinaryFunction(calcx, resx);
            int AA;
            AA = key.length();
            char myArray[AA++];
            strcpy(myArray, key.c_str());

            do
            {
                int text = strlen(myArray);
                MyCalculations = write(nf, &text, sizeof(int));
                MyCalculations = write(nf, &myArray, text);
            } while (true);

            close(nf);
            _exit(0);
        }
    } while (true);

    close(sk);

    return 0;
}

