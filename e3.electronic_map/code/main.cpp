#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>

#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <memory.h>

#define MC_UINT16(__data__) ( (__data__)>>8 | (__data__)<<8 )
#define MC_UINT32(__data__) ( (__data__)>>24 | (((__data__)>> 8)&0xFF00) | \
                              (((__data__)<< 8)&0x00FF0000) | (((__data__)<<24)&0xFF000000) )
/*
31-8	Reserved
7	路线名称有无Flag
6-4	岔路数
3-0	交叉Link列表示Class番号
*/
#define ROAD_NAME_FLAG(__data__) ((((__data__) & 0x80) >> 7) & 0x01)
#define BRUCH(__data__)          ((((__data__) & 0x70) >> 4) & 0x07)
#define DISPCLASS(__data__)      ((__data__) & 0x07)

typedef struct Link
{
    uint16_t size;
    uint32_t id;
    uint16_t nsize;
    uint32_t info;
    char*    name;
}Link;

void printLink(const Link* link)
{
    //#linkid=1234;roadnameflag=1;brunch=2;dispclass=3;roadname=青年大街# 	
    printf("#linkid=%d;roadnameflag=%d;brunch=%d;dispclass=%d;roadname=%s#", 
        link->id,
        ROAD_NAME_FLAG(link->info),
        BRUCH(link->info),
        DISPCLASS(link->info),
        ROAD_NAME_FLAG(link->info) ? link->name : "");
}

int readLink(FILE* fp, Link * link)
{
    size_t ret = 0;
    if (fread(&link->size, sizeof(uint16_t), 1, fp) != 1)
        return -1;
    link->size = MC_UINT16(link->size);
    if (link->size == 0xFFFF)
        return -1;

    if (fread(&link->id, sizeof(uint32_t), 1, fp) != 1)
        return -1;
    link->id = MC_UINT32(link->id);

    if (fread(&link->nsize, sizeof(uint16_t), 1, fp) != 1)
        return -1;
    link->nsize = MC_UINT16(link->nsize);

    if (fread(&link->info, sizeof(uint32_t), 1, fp) != 1)
        return -1;
    link->info = MC_UINT32(link->info);
    if (!ROAD_NAME_FLAG(link->info))
        link->nsize = 0;

    if (link->nsize > 0)
    {
        if ((link->name = (char*)malloc(link->nsize + 1))!=NULL)
        {
            if (fread(link->name, (link->nsize), 1, fp) != 1)
            {
                free(link->name);
                link->name = NULL;
                return -1;
            }
            link->name[link->nsize] = '\0';
        }
        else {
            return -1;
        }
    }

    uint16_t left = link->size - 12 - link->nsize;
    assert(left >= 0);
    fseek(fp, left, SEEK_CUR);

    return 0;
}

int main()
{
    FILE* fp = fopen("GTBL.dat", "rb");
    Link link;
    while (!feof(fp))
    {
        memset(&link, 0, sizeof(Link));
        if (readLink(fp, &link) != -1)
        {
            printLink(&link);
            printf("\n");
        }
        
    }
    fclose(fp);
    return 0;
}
