// MyTest_01.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <iostream>
#include <time.h>
using namespace std;

typedef struct tagRoundRecord
{
    int firstHit;   //第一次击球得分
    int secondHit;  //第二次击球得分
    int score;      //本轮总分
    int prizeFlag;  //奖励次数 0：没有全部击倒 1:补中; 2：全中;  
    int hitCount;   //本轮滚球次数 1 or 2
}RoundRecord;

#define FULLMARK 10
#define MAXROUND 12

#define FIRSTHIT 0
#define SECONDEHIT 1

RoundRecord roundRecords[MAXROUND];

/* 
函数功能：得到某轮某次滚球击倒的柱数
参数   ：roundIndex  --- 表示第几轮
        hitIndex    --- 表示第几次滚球
返回值：本次滚球击倒的柱数 
*/
int GetaHit(int roundIndex, int hitIndex)
{
    assert(roundIndex >= 0 && roundIndex < MAXROUND);
    assert(hitIndex == FIRSTHIT || hitIndex == SECONDEHIT);

    int hitedCnt = 0;
    if (FIRSTHIT == hitIndex)
    {
        if (rand() % 10 < 3)
            hitedCnt = 10;
        else
            hitedCnt = rand() % 10; //[0-9];
    }
    else if (SECONDEHIT == hitIndex)
    {
        int leftCnt = 10 - roundRecords[roundIndex].firstHit;
        if (rand() % 10 < 6)
        {
            hitedCnt = leftCnt;
        }
        else {
            hitedCnt = rand() % leftCnt; //[0, leftCnt-1]; 
        }
    }
    return hitedCnt;
}

/* 函数功能：输出最后的成绩 */
void OutputScore();

/* 函数功能：根据一次击倒柱数计算本轮以前各轮奖励后的分数 */
/* 参数：roundIndex : 本轮index
/*      hitScore   : 本轮某次击球分数
*/
void CalPrize(int roundIndex, int hitScore)
{
    assert(roundIndex >= 0 && roundIndex < MAXROUND);

    //最多能影响前两局的分数
    int startRound = (roundIndex - 2 < 0) ? 0 : (roundIndex - 2);
    for (int i = startRound; i < roundIndex; i++)
    {
        if (roundRecords[i].prizeFlag > 0) //如果有奖励
        {
            roundRecords[i].score += hitScore; //奖励分数
            roundRecords[i].prizeFlag--; //奖励次数减1
        }
    }
    return;
}

/* 函数功能：计算某轮滚球后的得分 */
/* 包括本轮和前几轮 */
void CalRoundScore(int roundIndex)
{
    assert(roundIndex >= 0 && roundIndex < MAXROUND);

    // 如果是前10轮滚球，计算奖励几次滚球
    if (roundIndex < MAXROUND - 2) //最后两轮不奖励？
    {
        // 全中，将奖励随后两次滚球的得分
        if (roundRecords[roundIndex].firstHit == FULLMARK)
        {
            roundRecords[roundIndex].prizeFlag = 2;
        }
        // 补中，将奖励随后一次滚球的得分
        else if (roundRecords[roundIndex].firstHit +
            roundRecords[roundIndex].secondHit == FULLMARK)
        {
            roundRecords[roundIndex].prizeFlag = 1;
        }
        else  // 没有全部击倒，不奖励
        {
            roundRecords[roundIndex].prizeFlag = 0;
        }
    }

    CalPrize(roundIndex, roundRecords[roundIndex].firstHit);
    if (roundRecords[roundIndex].hitCount == 2)
    {
        CalPrize(roundIndex, roundRecords[roundIndex].secondHit);
    }

    //本轮的分数等于，第一次和第二次击球(未击球则为0)的和
    roundRecords[roundIndex].score = roundRecords[roundIndex].firstHit
        + roundRecords[roundIndex].secondHit;

    return;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    memset(roundRecords, 0, MAXROUND * sizeof(RoundRecord));

    int i = 0;
    // 前10轮滚球，只记录每次滚球命中数和滚球次数
    for (i = 0; i < MAXROUND - 2; i++) //[0-9]
    {
        roundRecords[i].hitCount = 1;
        roundRecords[i].firstHit = GetaHit(i, FIRSTHIT);
        if (roundRecords[i].firstHit < FULLMARK)
        {
            roundRecords[i].hitCount = 2;
            roundRecords[i].secondHit = GetaHit(i, SECONDEHIT);
        }
        CalRoundScore(i);
    }

    //i = 10
    if (roundRecords[MAXROUND - 3].prizeFlag > 0) // 如果第10轮有奖励
    {
        // 第10轮全部击倒（全中或补中），奖励一次或两次滚球
        // 以下为第11轮
        roundRecords[i].hitCount = 1;
        roundRecords[i].firstHit = GetaHit(i, FIRSTHIT);
        //如果第10轮全部击倒，且第11轮第一次击球没有全中，再进行第二次击球
        if (2 == roundRecords[MAXROUND - 3].prizeFlag
            && roundRecords[i].firstHit < FULLMARK)
        {
            roundRecords[i].hitCount = 2;
            roundRecords[i].secondHit = GetaHit(i, SECONDEHIT);
        }
        CalRoundScore(i); //计算第11轮分数

        //如果第10轮和第11轮都全中，再奖励一次滚球，进行第12轮
        if (roundRecords[i].firstHit == FULLMARK
            && roundRecords[MAXROUND - 3].firstHit == FULLMARK)
        {
            i++; //i = 11
            roundRecords[i].hitCount = 1;
            roundRecords[i].firstHit = GetaHit(i, FIRSTHIT);
            CalRoundScore(i); //计算第12轮分数
        }
    }

    OutputScore();

    return 0;
}

void OutputScore()
{
    cout << "轮\t\t";
    for (int i = 0; i < MAXROUND; i++)
    {
        cout << "第" << i + 1 << "轮\t";
    }
    cout << endl;
    cout << "第一次得分\t";
    for (int i = 0; i < MAXROUND; i++)
    {
        cout << roundRecords[i].firstHit << "\t";
    }
    cout << endl;
    cout << "第二次得分\t";
    for (int i = 0; i < MAXROUND; i++)
    {
        if (roundRecords[i].hitCount == 2)
            cout << roundRecords[i].secondHit << "\t";
        else
            cout << "/\t";
    }
    cout << endl;
    cout << "本轮成绩\t";
    for (int i = 0; i < MAXROUND; i++)
    {
        cout << roundRecords[i].score << "\t";
    }
    cout << endl;

    cout << "累计总分\t";
    int sum = 0;
    for (int i = 0; i < MAXROUND - 2; i++)
    {
        sum += roundRecords[i].score;
        cout << sum << "\t";
    }
    cout << endl;
}
