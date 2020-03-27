# 程序说明

在一个应用程序中, 需要通过一个二维表查找对应的内容. 见下表: 
```  
            AA(COLUMN 0)    BB(COLUMN 1)    CC(COLUMN 2)    DD(COLUMN 3)	…   
AA(ROW 0)	A0	            A1	            A1	            A4              …   
BB(ROW 1)	A2	            A2	            A5	            A3              …   
CC(ROW 2)	A8	            A7	            A6	            A6              …   
DD(ROW 3)	A9	            A9	            A9	            A9              …   
…	        …	            …	            …	            …               …
```
当指定ROW值和COLUMN值时, 就能查找到对应的内容(A值),  这些表格有***多个***, 将作为程序的数据部分.

编制一个函数实现这个功能。    
```
	struct STableContext;
	unsigned int wuiGetTableItem(STableContext* pstTable, unsigned int uiRow, unsigned int uiColumn);
```

已知前提条件如下：
1.	表的数据内容是一个0到65535的数值.
2.	对应的二维表有多个, 而且每个表的 ROW_COUNT 和 COLUMN_COUNT 都不相同.
3.	原始表数据文件格式    
    a.	原始文件是文本格式, 文本行与上表对应.    
    b.	每行由多个单词构成, 每个单词之间由空格及制表符(TAB)的组合来分割.    
    c.	第一行的单词用于定义 COLUMN_VALUE 的命名    
    d.	从第二行开始, 每文本行对应二维表的一行. 第一个单词定义 ROW_VALUE 的命名.从第二个单词开始, 每个单词均由***数字***构成（即数据也被视为单词）描述二维表的内容.    
    e.	空文本行表示二维表格结束.    

下面是原始数据:    
```
    AA  BB  CC  DD    
AA	0   1   1   4    
BB	2   2   5   3    
CC	8   7   6   6    
DD	9   9   9	9    
```

4.	输出格式要求:
按C++语言格式描述. （包含 ROW_VALUE 定义, COLUMN_VALUE 定义及二维表数据结构）．     
下面是输出的例子

```
#define ROW_AA		    0
#define ROW_BB		    1
#define ROW_CC		    2
#define ROW_DD		    3
...
#define ROW_COUNT       4

#define COLUMN_AA       0
#define COLUMN_BB       1
#define COLUMN_CC       2
#define COLUMN_DD       3
...
#define COLUMN_COUNT    4

static <type> nuiTableData_a[][COLUMN_COUNT] = {
    0, 1, 1, 4,
    2, 2, 5, 3,
    8, 7, 6, 6,
    9, 9, 9, 9
};

STableContext nstTestTable_a =
{ ROW_COUNT, COLUMN_COUNT, nuiTableData_a };

...
// other tables

```
5.	原始表格数据是正确的, 不必考虑对原始数据的严格分析判断
6.	根据原始数据计算出 ROW_COUNT 和 COLUMN_COUNT 的值
7.	COLUMN VALUE 从0开始, 依次递增.
8.	ROW VALUE 的定义和 COLUMN VALUE 定义类似, 但需要进行压缩判断.如果两行内容相同, ROW VALUE值也应该一样.
例如原始数据:
```
    C1  C2  C3  C4  C5
PP	0	0	1	1	2
RR	1	2	2	1	5
QQ	0	1	2	3	4
SS	1	2	2	1	5
TT	9	9	8	8	7

...
```
那么输出的结果应是:
```
#define COLUMN_C1 0
#define COLUMN_C2 1
#define COLUMN_C3 2
#define COLUMN_C4 3
#define COLUMN_C5 4
#define COLUMN_COUNT 5

#define	ROW_PP	0
#define	ROW_RR	1
#define ROW_QQ	2
#define	ROW_SS	ROW_RR
#define	ROW_TT	3
...
static <type> nuiTableData_a[][COLUMN_COUNT] = {
    0, 0, 1, 1, 2,
    1, 2, 2, 1, 5,
    0, 1, 2, 3, 4,
    9, 9, 8, 8, 7
};

STableContext nstTestTable_a =
{ ROW_COUNT, COLUMN_COUNT, nuiTableData_a };

```

题目:
1.	定义STableContext
2.	编写wuiGetTableItem
3.	编写一个程序。 读取一个文本表格文件, 并生成另一种格式的文件(C++语言头文件格式), 同时将表格的行列索引(AAA, BBB, CCC…,即ROW VALUE, COLUMN VALUE)以C++语言的格式进行定义.
