#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

typedef int BInt[501];

void zero(BInt a);
void read(char* str, BInt num);
void print(BInt num);
int compareBInt(BInt a, BInt b);

// c = a + b
// support a = a + b
void add(BInt a, BInt b, BInt c)
{
    int x = 0;
    int i = 0;

    if (c != a) zero(c);

    for (i = 1; i <= a[0] || i <= b[0]; i++)
    {
        c[i] = a[i] + b[i] + x;
        x = c[i] / 10;
        c[i] = c[i] % 10;
    }
    c[i] = x;

    while (c[i] == 0 && i > 1)
        i--;
    c[0] = i;
}

// c = a - b assert(a >= b)
// support a = a - b
void sub(BInt a, BInt b, BInt c)
{
    int x = 0;

    if (c != a) zero(c);

    for (int i = 1; i <= a[0]; i++)
    {
        c[i] = a[i] - x - b[i];
        x = c[i] < 0 ? 1 : 0;
        c[i] += x * 10;
    }

    int len = a[0];
    while (c[len] == 0 && len > 1)
        len--;
    c[0] = len;
}

// c = a x b (assert(c!=a))
void mult(BInt a, BInt b, BInt c)
{
    int x = 0;

    zero(c);

    // ai x bi 会作用在结果的 (i-1)+(j-1)+1 位， 然后根据加法进位规则即
    for (int i = 1; i <= b[0]; i++)
    {
        x = 0;
        for (int j = 1; j <= a[0]; j++)
        {
            c[i + j - 1] += (a[j] * b[i] + x);
            x = c[i + j - 1] / 10;
            c[i + j - 1] %= 10;
        }
        c[a[0] + i] = x;
    }

    int len = a[0] + b[0];
    while (c[len] == 0 && len > 1)
        len--;
    c[0] = len;
}

// a = a * b + c
void mult(BInt a, int b, int c)
{
    int x = c;
    int i = 0;

    for (i = 1; i <= a[0]; i++)
    {
        a[i] = a[i] * b + x;
        x = a[i] / 10;
        a[i] %= 10;
    }

    // note, the left x may more than 9.
    while (x)
    {
        a[i++] = x % 10;
        x /= 10;
    }

    while (a[i] == 0 && i > 1)
        i--;
    a[0] = i;
}

void div(BInt a, BInt b, BInt c, BInt d)
{
    /*
            019
           ____
       50_| 986
            50
            ---
            486
            450
            ---
             36
    */

    int i = 0;

    BInt x; //余下的部分
    zero(x);

    for (i = a[0]; i >= 1; i--)
    {
        mult(x, 10, a[i]);
        int ret = 0;
        while (compareBInt(x, b) >= 0)
        {
            sub(x, b, x);
            ret++;
        }
        c[i] = ret; // maybe some zero at front
    }

    i = a[0];

    while (c[i] == 0 && i > 1)
        i--;

    c[0] = i;

    for (i = 0; i <= x[0]; i++)
        d[i] = x[i];
}

BInt ag, bg, cg, dg;

int main()
{
    char buf[500];
    cin >> buf;
    read(buf, ag);
    cin >> buf;
    read(buf, bg);

    add(ag, bg, cg);
    print(cg);

    return 0;
}

void zero(BInt a)
{
    memset(a, 0, sizeof(BInt));
    a[0] = 1;
}

void read(char* str, BInt num)
{
    int i = 0;
    int len = strlen(str);
    num[0] = len;

    for (i = 1; i <= len; i++)
    {
        if (isdigit(str[len - i]))
            num[i] = str[len - i] - '0';
        else if (isupper(str[len - i]))
            num[i] = str[len - i] - 'A' + 10;
        else if (islower(str[len - i]))
            num[i] = str[len - i] - 'a' + 10;
    }

    do
    {
        i--;
    } while (num[i] == 0 && i > 1);

    num[0] = i;
}

void print(BInt num)
{
    static const char hex[] = { 'A', 'B', 'C', 'D', 'E', 'F', };
    for (int i = num[0]; i >= 1; i--)
    {
        if (num[i] < 10)
            printf("%d", num[i]);
        else {
            printf("%c", hex[num[i] - 10]);
        }
    }
}

// a >= 0, b >= 0
int compareBInt(BInt a, BInt b)
{
    if (a[0] > b[0]) // a > b;
        return 1;

    if (a[0] < b[0])
        return -1;

    for (int i = a[0]; i >= 1; i--)
        if (a[i] != b[i])
            return a[i] - b[i];

    return 0;
}

void add(BInt a, BInt b, int base, BInt c)
{
    int x = 0;
    int i = 0;

    if (c != a)
        zero(c);

    for (i = 1; i <= a[0] || i <= b[0]; i++)
    {
        c[i] = a[i] + b[i] + x;
        x = c[i] / base;
        c[i] = c[i] % base;
    }

    c[i] = x;

    while (c[i] == 0 && i > 1)
        i--;

    c[0] = i;
}

void reverse(BInt a, BInt b)
{
    int i = 0;
    for (i = 1; i <= a[0]; i++)
        b[i] = a[a[0] - i + 1];

    i = a[0];

    while (b[i] == 0 && i > 1)
        i--;

    b[0] = i;
}
