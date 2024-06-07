#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <dirent.h>
#include <cstdio>
#include <conio.h>

using namespace std;

bool isChar(int keyCode)
{
    return (keyCode >= 32 && keyCode <= 126);
}

void clear()
{
    system("cls");
}

string pathGetName(string path)
{
    string result = "";
    int length = path.size();
    bool isFirst = true;
    for (int i = length - 1; i >= 0; i--)
    {
        if (path[i] != '\\')
        {
            result = path[i] + result;
            isFirst = false;
        }
        else if (!isFirst)
            break;
    }
    return result;
}

string PathModifierOneSlash(string p)
{
    int length = p.size();
    if (length <= 0)
        return "";
    string result = "";
    bool isSlash = false;
    for (int i = 0; i < length; i++)
    {
        if (isSlash)
        {
            if (p[i] == '\\')
            {
                continue;
            }
            else
            {
                isSlash = false;
                result += p[i];
            }
        }
        else if (p[i] == '\\')
        {
            isSlash = true;
            result += p[i];
        }
        else
            result += p[i];
    }
    int rLength = result.size();
    if (result[rLength - 1] != '\\')
        result += "\\";
    return result;
}

string PathModifier(string p)
{
    int length = p.size();
    if (length <= 0)
        return "";
    string result = "";
    bool isSlash = false;
    for (int i = 0; i < length; i++)
    {
        if (isSlash)
        {
            if (p[i] == '\\')
            {
                result += p[i];
            }
            else
            {
                result += "\\";
                result += p[i];
            }
            isSlash = false;
        }
        else if (p[i] == '\\')
        {
            isSlash = true;
            result += p[i];
        }
        else
            result += p[i];
    }
    int rLength = result.size();
    if (result[rLength - 1] != '\\')
        result += "\\\\";
    else if (result[rLength - 2] != '\\')
        result += "\\";
    return result;
}

string pathCombine(string path, string sub)
{
    return PathModifier(path) + pathGetName(sub);
}

string pathBack(string path)
{
    string result = path;
    int length = path.size();
    bool isFirst = true;
    for (int i = length - 1; i >= 0; i--)
    {
        if (path[i] == ':')
            break;
        if (isFirst && path[i] == '\\')
        {
            result.erase(i, 1);
        }
        else if (path[i] != '\\')
        {
            isFirst = false;
            result.erase(i, 1);
        }
        else
            break;
    }
    return result;
}

string ToLower(string inp)
{
    string r = "";
    for (int i = 0; i < inp.size(); i++)
    {
        r += towlower(inp[i]);
    }
    return r;
}

string Format(string inp, int length = 40)
{
    string result = "";
    if (inp.size() < length)
    {
        string space = "";
        for (int i = inp.size(); i < length; i++)
            space += " ";
        result = inp + space;
    }
    else if (inp.size() > length)
    {
        result = inp.substr(0, length - 4) + "... ";
    }
    else
    {
        result = inp;
    }
    return result;
}

string BackSpace(string input)
{
    if (input.size() > 0)
        return input.substr(0, input.size() - 1);
    else
        return input;
}

vector<string> selectMathCase(vector<string> inp, string key, bool caseSensitive)
{
    vector<string> r;
    try
    {
        regex integer_expr(key);
        for (int i = 0; i < inp.size(); i++)
        {
            if (caseSensitive && regex_match(inp[i], integer_expr))
            {
                r.push_back(inp[i]);
            }
            else if (!caseSensitive && regex_match(inp[i], integer_expr))
            {
                r.push_back(inp[i]);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return r;
}

string current_working_directory()
{
    char *cwd = _getcwd(0, 0); // **** microsoft specific ****
    string working_directory(cwd);
    free(cwd);
    return working_directory;
}

vector<string> GetSubFile(string path)
{
    vector<string> r;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string name = ent->d_name;
            if (name == "." || name == "..")
                continue;
            r.push_back(name);
        }
    }
    closedir(dir);
    return r;
}

template <typename T>
void PrintVector(std::vector<T> arr, std::string between = "\t")
{
    for (int i = 0; i < arr.size(); i++)
    {
        std::cout << arr.at(i) << between;
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &out, std::vector<T> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << vec.at(i) << "\t";
    }
    return out;
}

string replace(string inp, string old_str, string new_str)
{
    string result = "";
    size_t startPos = 0;
    size_t lenOldStr = old_str.length();
    size_t posFind;
    while ((posFind = inp.find(old_str, startPos)) != std::string::npos)
    {
        result += inp.substr(startPos, posFind - startPos) + new_str;
        startPos = posFind + lenOldStr;
    }
    result += inp.substr(startPos);
    return result;
}

void printHeader()
{
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << Format("index", 7) << Format("Name") << Format("Path") << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
}

void onlineMode(string path, string selector)
{
    char status = '\0';
    vector<string> listFileSelected;
    while (true)
    {
        clear();
        cout << PathModifierOneSlash(path) << "\nSelector>" << selector << endl;

        auto listFile = GetSubFile(path);
        listFileSelected = selectMathCase(listFile, selector, true);

        printHeader();
        for (int i = 0; i < listFileSelected.size(); i++)
        {
            cout << Format(to_string(i + 1) + ") ", 7) << Format(listFileSelected[i]) << Format(PathModifierOneSlash(pathCombine(path, listFileSelected[i]))) << endl;
        }
        printHeader();
        cout << PathModifierOneSlash(path) << "\nSelector>" << selector << endl;
        int readkey = getch();
        if (readkey == 27)
        {
            if (status == 'E')
                return;
            status = 'E';
            continue;
        }
        else if (readkey == 13)
        {
            if (status == 'C')
                break;
            status = 'C';
            continue;
        }
        else if (readkey == 224)
        {
            getch();
        }
        else if (readkey == 8)
        {
            selector = BackSpace(selector);
        }
        else
        {
            if (isChar(readkey))
                selector += (char)readkey;
        }
        status = '\0';
    }
    if (status == 'C')
    {
        for (size_t i = 0; i < listFileSelected.size(); i++)
        {
            string pathfile = BackSpace(PathModifierOneSlash(pathCombine(path, listFileSelected[i])));
            string command = "ren \"" + pathfile + "\" \"" + to_string(i) + ".jpg\"";
            cout << command << endl;
            system(command.c_str());
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc >= 2 && ToLower(argv[1]) == "-o")
    {
        onlineMode("F:\\Projects\\Py\\movie_site\\movie_site\\media\\post_covers", "");
        return 0;
    }
    if (argc <= 2)
    {
        cout << "Error argoment";
        return 0;
    }
    // cout << argv[0] << endl;
    string selectKey = argv[1];
    if (selectKey[0] != '-' || selectKey.length() < 2)
    {
        cout << "Error argoment\nError select argoment";
        return 0;
    }
    selectKey = selectKey.substr(1);
    selectKey = selectKey == "*" ? "(.*)" : selectKey;
    selectKey = replace(selectKey, "**", "(.*)");

    cout << "selectKey:" << selectKey << endl;

    // auto currentDirectory = current_working_directory();
    auto currentDirectory = "D:\\New folder";
    auto listFile = GetSubFile(currentDirectory);
    auto listFileSelected = selectMathCase(listFile, selectKey, true);
    PrintVector(listFileSelected, "\t");
    return 0;
}