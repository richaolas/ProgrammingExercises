/*
creat by renjch 2020/03/31
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

string trim(const string& s) {
    string tmp = s;
    if (tmp.empty())
        return tmp;
    tmp.erase(0, tmp.find_first_not_of(" \t"));
    tmp.erase(tmp.find_last_not_of(" \t") + 1);
    return tmp;
}

void split(const string& s, const string& delim, vector<string>& ret) {
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while (index != string::npos) {
        string elem = s.substr(last, index - last);
        if (delim.find(elem)== string::npos)
            ret.push_back(elem);
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0)
    {
        string elem = s.substr(last, index - last);
        if (delim.find(elem) == string::npos)
            ret.push_back(elem);
    }
}

string nextLetter(const string& letter)
{
    int x = 1;
    string ret = letter;
    for (auto it = ret.rbegin(); it != ret.rend(); ++it)
    {
        if (x == 0)
            break;
        int v = (*it - 'a') + x;
        x = v / 26;
        v = v % 26;
        *it = v + 'a';
    }
   
    if (x)
        ret = "a" + ret;
    return ret;
}

class Table
{
    friend class TableParser;
public:
    Table() {}
    ~Table() {}   
    
    void outputEncodeTable(const string& tag) const
    {
        int columnCnt = _header.size();
        int rowCnt = _rowName.size();

        compress();

        for (int i = 0; i < columnCnt; i++)
        {
            cout << "#define COLUMN_" << _header[i] << " " << i << endl;
        }
        cout << "#define COLUMN_COUNT" << " " << columnCnt << endl;
        cout << "\n";

        int rowDefineValue = 0;
        for (int i = 0; i < rowCnt; i++)
        {
            cout << "#define ROW_" << _rowName[i] << " ";
            if (_compressedRowIdx[i] == i)
                cout << rowDefineValue++;
            else
                cout << "ROW_" << _rowName[_compressedRowIdx[i]];
            cout << "\n";
        }
        cout << "#define ROW_COUNT" << " " << rowDefineValue << endl;
        cout << "\n";

        cout << "static <type> nuiTableData_" << tag << "[][COLUMN_COUNT] = {\n";
        for (int row = 0; row < rowCnt; row++)
        {
            if (_compressedRowIdx[row] == row)
            {
                cout << "\t";
                for (int col = 0; col < columnCnt; col++)
                    cout << _rows[row][col] << ", ";
                cout << "\n";
            }
        }
        cout << "};\n\n";

        cout << "STableContext nstTestTable_" << tag << " = {ROW_COUNT, COLUMN_COUNT, nuiTableData_" << tag << "};";
        cout << "\n\n";
    }
   
    void clear() 
    { 
        _header.clear();
        _rowName.clear();
        _rows.clear();
    }
    
    bool hasHeader() const
    {
        return _header.size() > 0;
    }

private:
    void compress() const
    {
        _compressedRowIdx.clear();
        for (int i = 0; i < _rows.size(); i++)
        {
            int idx = i;
            for (int j = 0; j < i; j++)
            {
                if (_rows[i] == _rows[j])
                {
                    idx = j;
                    break;
                }
            }
            _compressedRowIdx.push_back(idx);
        }
    }

    std::vector<std::string>      _header;
    std::vector<std::string>      _rowName;
    std::vector<std::vector<int>> _rows;
    mutable std::vector<int>      _compressedRowIdx;
};

class TableParser
{
public:
    TableParser(string path)
    {
        file.open(path);
    }

    ~TableParser()
    {
        file.close();
    }

    bool nextTable(Table & table)
    {
        table.clear();
        string line;
        std::vector<std::string> raw;

        while (!file.eof())
        {
            getline(file, line);
            line = trim(line);

            if (line.empty())
            {
                if (table.hasHeader())
                    break;
                continue;
            }

            raw.clear();
            split(line, " \t", raw);

            if (!table.hasHeader())
            {
                swap(table._header, raw); // or table._header = move(raw);
            }
            else {                
                table._rowName.push_back(raw[0]);
                table._rows.push_back(vector<int>());
                transform(next(raw.begin()), raw.end(), 
                    back_inserter(*prev(table._rows.end())),
                    [](const string& s)->int {return stol(s, 0, 10);});
            }
        }

        return table.hasHeader();
    }

private:
    ifstream file;
};

int main()
{
    TableParser parser("sample.txt");
    string tag = "a";
    while (true)
    {
        Table table;
        if (parser.nextTable(table))
            table.outputEncodeTable(tag);
        else
            break;
        tag = nextLetter(tag);
    }

	return 0;
}
