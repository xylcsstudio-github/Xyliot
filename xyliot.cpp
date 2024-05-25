#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <ncurses.h>

// 函数：加载文件内容到lines向量
void loadFile(const std::string& filename, std::vector<std::wstring>& lines) {
    // 打开文件
    std::wifstream infile(filename);
    // 设置文件流使用UTF-8编码
    infile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstring line;
    // 逐行读取文件内容
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
}

// 函数：将lines向量的内容保存到文件
void saveFile(const std::string& filename, const std::vector<std::wstring>& lines) {
    // 打开文件
    std::wofstream outfile(filename);
    // 设置文件流使用UTF-8编码
    outfile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    // 逐行写入文件
    for (const auto& line : lines) {
        outfile << line << std::endl;
    }
}

// 函数：刷新窗口内容
void refreshWindow(const std::vector<std::wstring>& lines, int startLine, int startCol, int x, int y) {
    // 清屏
    clear();
    int maxY, maxX;
    // 获取屏幕大小
    getmaxyx(stdscr, maxY, maxX);

    // 逐行显示文件内容，从startLine开始
    for (int i = 0; i < maxY && (startLine + i) < lines.size(); ++i) {
        move(i, 0);
        // 显示从startCol开始的内容
        if (lines[startLine + i].size() > startCol) {
            printw("%ls", lines[startLine + i].substr(startCol, maxX).c_str());
        }
    }

    // 移动光标到(x, y)
    move(y - startLine, x - startCol);
    refresh();
}

int main() {
    // 设置本地化，支持宽字符
    setlocale(LC_ALL, "");
    // 初始化ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // 提示用户输入文件名
    printw("Welcome to Linux Text Editor Xyliot!\n Version 1.0.r1.b7\nTips1:After enter filename, you can write anythink. Press F1 to save and exit（Your files and programs are in the same directory）\nTips2:If the file name you input already exists, you can modify that file.\nTips3:Although I know this is quite basic, this version can only do it this way. Move the program anywhere to edit files anywhere.\nEnter filename: ");


    echo();
    char filename[256];
    getstr(filename);
    noecho();

    // 清屏
    clear();
    refresh();

    // 加载文件内容到lines向量
    std::vector<std::wstring> lines;
    std::ifstream infile(filename);
    if (infile.good()) {
        loadFile(filename, lines);
    } else {
        // 如果文件不存在，则创建一个新文件
        std::ofstream outfile(filename);
        if (!outfile) {
            printw("Failed to create file.");
            getch();
            endwin();
            return 1;
        }
        lines.push_back(L"");
    }

    // 初始化光标位置和滚动起始位置
    int x = 0, y = 0;
    int startLine = 0;
    int startCol = 0;
    if (!lines.empty()) {
        // 刷新窗口显示内容
        refreshWindow(lines, startLine, startCol, x, y);
    }

    wint_t ch;
    // 主循环，处理用户输入
    while (true) {
        get_wch(&ch);
        if (ch == KEY_F(1)) { // F1 键保存并退出
            break;
        }

        int maxY, maxX;
        // 获取屏幕大小
        getmaxyx(stdscr, maxY, maxX);

        // 处理不同按键的功能
        switch (ch) {
            case KEY_BACKSPACE:
            case 127:
                // 处理退格键删除字符
                if (x > 0) {
                    lines[y].erase(--x, 1);
                    if (x < startCol) startCol--;
                    refreshWindow(lines, startLine, startCol, x, y);
                } else if (y > 0) {
                    // 如果在行首，连接上一行
                    x = lines[y - 1].size();
                    lines[y - 1] += lines[y];
                    lines.erase(lines.begin() + y);
                    y--;
                    if (y < startLine) startLine--;
                    if (x - startCol >= maxX) startCol = x - maxX + 1;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_DC:
                // 处理删除键删除字符
                if (x < lines[y].size()) {
                    lines[y].erase(x, 1);
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case '\n':
                // 处理回车键，拆分当前行
                lines.insert(lines.begin() + y + 1, lines[y].substr(x));
                lines[y] = lines[y].substr(0, x);
                y++;
                x = 0;
                if (y - startLine >= maxY) startLine++;
                startCol = 0;
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_UP:
                // 处理向上键移动光标
                if (y > 0) {
                    y--;
                    if (y < startLine) startLine--;
                    x = std::min(x, (int)lines[y].size());
                    if (x < startCol) startCol = x;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_DOWN:
                // 处理向下键移动光标
                if (y < lines.size() - 1) {
                    y++;
                    if (y - startLine >= maxY) startLine++;
                    x = std::min(x, (int)lines[y].size());
                    if (x - startCol >= maxX) startCol = x - maxX + 1;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_LEFT:
                // 处理向左键移动光标
                if (x > 0) {
                    x--;
                    if (x < startCol) startCol--;
                } else if (y > 0) {
                    y--;
                    if (y < startLine) startLine--;
                    x = lines[y].size();
                    if (x - startCol >= maxX) startCol = x - maxX + 1;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_RIGHT:
                // 处理向右键移动光标
                if (x < lines[y].size()) {
                    x++;
                    if (x - startCol >= maxX) startCol++;
                } else if (y < lines.size() - 1) {
                    y++;
                    if (y - startLine >= maxY) startLine++;
                    x = 0;
                    startCol = 0;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            default:
                // 处理其他字符输入，插入字符
                if (ch < 128) {
                    lines[y].insert(lines[y].begin() + x, ch);
                    x++;
                    if (x - startCol >= maxX) startCol++;
                } else {
                    wchar_t wch = ch;
                    lines[y].insert(lines[y].begin() + x, wch);
                    x++;
                    if (x - startCol >= maxX) startCol++;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
        }
    }

    // 保存文件内容
    saveFile(filename, lines);
    endwin(); // 结束ncurses模式
    return 0;
}
