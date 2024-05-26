#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <ncurses.h>
#include <unistd.h> // 用于获取当前工作目录

// 函数：加载文件内容到lines向量
void loadFile(const std::string& filename, std::vector<std::wstring>& lines) {
    std::wifstream infile(filename);
    infile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstring line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
}

// 函数：将lines向量的内容保存到文件
void saveFile(const std::string& filename, const std::vector<std::wstring>& lines) {
    std::wofstream outfile(filename);
    outfile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    for (const auto& line : lines) {
        outfile << line << std::endl;
    }
}

// 函数：刷新窗口内容
void refreshWindow(const std::vector<std::wstring>& lines, int startLine, int startCol, int x, int y) {
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    for (int i = 0; i < maxY && (startLine + i) < lines.size(); ++i) {
        move(i, 0);
        // 显示行号，使用天蓝色
        attron(COLOR_PAIR(1));
        printw("%4d ", startLine + i + 1);
        attroff(COLOR_PAIR(1));

        if (lines[startLine + i].size() > startCol) {
            printw("%ls", lines[startLine + i].substr(startCol, maxX - 5).c_str());
        }
    }

    move(y - startLine, x - startCol + 5); // 移动光标到 (x, y)，并考虑行号的宽度
    refresh();
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK); // 初始化颜色对1：前景色为天蓝色，背景色为黑色

    raw();
    keypad(stdscr, TRUE);
    noecho();

    // 提示用户输入文件路径和文件名
    printw("Welcome to Linux Text Editor Xyliot!\n Version 1.0.r1.b12\nTips1:After enter filename, you can write anything. Press F1 to save and exit（Your files and programs are in the same directory）\nTips2:If the file name you input already exists, you can modify that file.\nTips3: PgUP and PgDn can move to the top and bottom.\nTips4:You can input either absolute or relative file paths when prompted.\nTips5:Move the compiled program to /usr/local/bin so that you can run it from anywhere just by typing its name next time!\nMore please read readme.txt\nEnter file path and filename: ");
    echo();
    char filename[256];
    getstr(filename);
    noecho();

    clear();
    refresh();

    std::vector<std::wstring> lines;
    std::ifstream infile(filename);
    if (infile.good()) {
        loadFile(filename, lines);
    } else {
        std::ofstream outfile(filename);
        if (!outfile) {
            printw("Failed to create file.");
            getch();
            endwin();
            return 1;
        }
        lines.push_back(L"");
    }

    int x = 0, y = 0;
    int startLine = 0;
    int startCol = 0;
    if (!lines.empty()) {
        refreshWindow(lines, startLine, startCol, x, y);
    }

    wint_t ch;
    bool exitRequested = false; // 添加一个标志，用于控制循环退出
    while (!exitRequested) { // 修改退出条件为循环标志为真时退出
        get_wch(&ch);
        if (ch == KEY_F(1)) { // F1 键保存并退出
            exitRequested = true; // 设置循环标志为真，退出循环
        }

        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);

        switch (ch) {
            case KEY_BACKSPACE:
            case 127:
                if (x > 0) {
                    lines[y].erase(--x, 1);
                    if (x < startCol) startCol--;
                    refreshWindow(lines, startLine, startCol, x, y);
                } else if (y > 0) {
                    x = lines[y - 1].size();
                    lines[y - 1] += lines[y];
                    lines.erase(lines.begin() + y);
                    y--;
                    if (y < startLine) startLine--;
                    if (x - startCol >= maxX - 5) startCol = x - (maxX - 5) + 1;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_DC:
                if (x < lines[y].size()) {
                    lines[y].erase(x, 
1);
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case '\n':
                lines.insert(lines.begin() + y + 1, lines[y].substr(x));
                lines[y] = lines[y].substr(0, x);
                y++;
                x = 0;
                if (y - startLine >= maxY) startLine++;
                startCol = 0;
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_UP:
                if (y > 0) {
                    y--;
                    if (y < startLine) startLine--;
                    x = std::min(x, (int)lines[y].size());
                    if (x < startCol) startCol = x;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_DOWN:
                if (y < lines.size() - 1) {
                    y++;
                    if (y - startLine >= maxY) startLine++;
                    x = std::min(x, (int)lines[y].size());
                    if (x - startCol >= maxX - 5) startCol = x - (maxX - 5) + 1;
                    refreshWindow(lines, startLine, startCol, x, y);
                }
                break;
            case KEY_LEFT:
                if (x > 0) {
                    x--;
                    if (x < startCol) startCol--;
                } else if (y > 0) {
                    y--;
                    if (y < startLine) startLine--;
                    x = lines[y].size();
                    if (x - startCol >= maxX - 5) startCol = x - (maxX - 5) + 1;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_RIGHT:
                if (x < lines[y].size()) {
                    x++;
                    if (x - startCol >= maxX - 5) startCol++;
                } else if (y < lines.size() - 1) {
                    y++;
                    if (y - startLine >= maxY) startLine++;
                    x = 0;
                    startCol = 0;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_PPAGE:
                // Page Up 键，将光标和屏幕移动到文件顶部
                y = 0;
                startLine = 0;
                x = 0;
                startCol = 0;
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            case KEY_NPAGE:
                // Page Down 键，将光标和屏幕移动到文件底部
                y = lines.size() - 1;
                startLine = std::max(0, (int)lines.size() - maxY);
                x = lines[y].size();
                startCol = std::max(0, x - (maxX - 5) + 1);
                refreshWindow(lines, startLine, startCol, x, y);
                break;
            default:
                if (ch < 128) {
                    lines[y].insert(lines[y].begin() + x, ch);
                    x++;
                    if (x - startCol >= maxX - 5) startCol++;
                } else {
                    wchar_t wch = ch;
                    lines[y].insert(lines[y].begin() + x, wch);
                    x++;
                    if (x - startCol >= maxX - 5) startCol++;
                }
                refreshWindow(lines, startLine, startCol, x, y);
                break;
        }
    }

    saveFile(filename, lines);
    endwin();
    return 0;
}
