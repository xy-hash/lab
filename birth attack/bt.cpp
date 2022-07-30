#include <iostream>
#include <chrono>
#include"sm3.h"
using namespace std;

#define N 100000

int main()
{
    srand((unsigned)time(NULL));
    int cnt = 0;
    int IVset[N];
    string* x = new string[N];
    while (cnt <= N - 1)
    {
        unsigned char arr1[32];
        for (int i = 0; i < 32; i++)
        {
            arr1[i] = (char)rand() % 255;
            x[cnt].push_back(arr[i]);
        }
        sm3(arr1, 32);
        IVset[cnt] = IV[0];
        for (int i = 0; i < cnt; i++)
        {
            if (IVset[i] == IVset[cnt] && x[i] != x[cnt])
            {
                for (int j = 0; j < 32; j++)
                {
                    cout << hex << (int)x[i][j];
                }
                cout << endl ;
                for (int j = 0; j < 32; j++)
                {
                    cout << hex << (int)x[cnt][j];
                }
                cout << endl << endl;
            }
        }
        cnt++;
    }

}
