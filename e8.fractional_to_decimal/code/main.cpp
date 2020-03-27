#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    printf("请输入除法 m/n ,n=0 退出 :\n");
    while (1) {
        int m, n;
        if (2 != scanf("%d/%d", &m, &n))
        {
            cout << "Error input format!\n";
            continue;
        }

        if (n == 0)
            break;

        int a = m / n;
        int b = m % n;

        map<int, int> yu;
        vector<int> ret;
        int idx = 0;

        while (b && !yu.count(b))
        {
            yu[b] = idx++;
            ret.push_back(b * 10 / n);
            b = b * 10 % n;
        } 
        
        cout << m << "/" << n << "=" << a << ".";
        for (int i = 0, beg = yu[b]; i < ret.size(); i++)
        {
            if (i == beg && b!=0)
                cout << "(";
            cout << ret[i];
        }
        if (b)
            cout << ")";
        cout << "\n\n" << endl;
    }

    return 0;
}