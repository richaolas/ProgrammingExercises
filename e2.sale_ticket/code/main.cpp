// Ticket_C.cpp : 定义控制台应用程序的入口点。

//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>

#define SEATINSPACE 4
#define MAXSALECNT 4
#define SPACECNT 20

typedef enum Status{ENOUGH_NO = 0, ENOUGH_SEP, ENOUGH_ALL, UNKNOWN}Status;

typedef struct tagSpace
{
    int num; //隔间左上角座位号
    int left; //隔间剩余车票数量
    bool hasTicket[SEATINSPACE]; //票是有票 true:有票 false：售完
}Space;

typedef struct tagTicketSale
{
    int ticketNum[MAXSALECNT]; //座位号
    int saleCnt; //卖出几张
}TicketSale;

void mergeTicketSale(TicketSale* sale, const TicketSale* mergeSale)
{
    for (int i = 0; i < mergeSale->saleCnt; i++)
        sale->ticketNum[sale->saleCnt++] = mergeSale->ticketNum[i];
}

int getSeatNum(const Space* pSpace, int idx)
{
    const int offset[SEATINSPACE] = { 0, 1, 4, 5 };
    return pSpace->num + offset[idx];
}

void initCarriage(Space* pSpace, int cnt)
{
    for (int i = 0; i < cnt; i++)
    {
        pSpace[i].num = 4 * i + (i % 2 ? -1 : 1);
        for (int t = 0; t < SEATINSPACE; t++)
            pSpace[i].hasTicket[t] = true;
        pSpace[i].left = SEATINSPACE;
    }
}

Status getSaleSpaceIndex(Space* pSpace, int cnt, int saleCnt, int * index)
{
    int left = 0;
    *index = -1;
    for (int i = 0; i < cnt; i++)
    {
        left += pSpace[i].left;
        if (pSpace[i].left >= saleCnt)
        {
            *index = i;
            return ENOUGH_ALL;
        }
    }
        
    return left >= saleCnt ? ENOUGH_SEP : ENOUGH_NO;
}

TicketSale SaleTicket(Space* pSpace, int cnt)
{
    assert(cnt >= 0 && cnt <= MAXSALECNT); //cnt<=4 && cnt>=1
    
    TicketSale ts = { {0,},0 };
    
    for (int i = 0; i < SEATINSPACE && ts.saleCnt != cnt; i++)
        if (pSpace->hasTicket[i])
        {
            pSpace->hasTicket[i] = false; //买票
            pSpace->left--;
            ts.ticketNum[ts.saleCnt++] = getSeatNum(pSpace, i);
        }

    return ts;
}

void ShowCarriage(const Space* pSpace, int cnt);

int main()
{
    Space spaces[SPACECNT];
    initCarriage(spaces, SPACECNT);

    int cnt = 0;
    while (true)
    {
        ShowCarriage(spaces, SPACECNT);
        if (1 != scanf(" %d", &cnt) || cnt > 4 || cnt < 0)
        {
            fflush(stdin);
            continue;
        }    

        int index = -1;
        Status status = getSaleSpaceIndex(spaces, SPACECNT, cnt, &index);

        TicketSale ts = { {0,},0 };

        if (status == ENOUGH_ALL)
        {
            ts = SaleTicket(spaces + index, cnt);
        }
        else if (status == ENOUGH_SEP)
        {
            for (int i = 0; i < cnt; i++)
            {
                getSaleSpaceIndex(spaces, SPACECNT, 1, &index);
                TicketSale one = SaleTicket(spaces + index, cnt);
                mergeTicketSale(&ts, &one);
            }
        }
        if (ts.saleCnt == cnt)
        {
            printf("售票为：");
            for (int t = 0; t < cnt; t++)
            {
                printf("%d ", ts.ticketNum[t]);
            }
            printf("\n");
        }
        else {
            printf("余票不足!\n");
        }
    }

    return 0;
}

void ShowCarriage(const Space* pSpace, int cnt)
{
    for (int n = 0; n < 4; n++)
    {
        int i = (n < 2) ? 0 : 1;
        for (; i < cnt; i += 2)
        {
            if (pSpace[i].hasTicket[n % 2])
                printf("%2d ", getSeatNum(pSpace + i, n % 2));
            else
                printf("-- ");
            if (pSpace[i].hasTicket[n % 2 + 2])
                printf("%2d |", getSeatNum(pSpace + i, n % 2 + 2));
            else
                printf("-- |");
        }
        printf("\n");
        if (n == 1)
            printf("\n");
    }
    printf("\n");
}
