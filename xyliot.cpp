#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <ncurses.h>

void loadFile(const std::string& filename, std::vector<std::wstring>& lines) {
    std::wifstream infile(filename);
    infile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstring line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
}

void saveFile(const std::string& filename, const std::vector<std::wstring>& lines) {
    std::wofstream outfile(filename);
    outfile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    for (const auto& line : lines) {
        outfile << line << std::endl;
    }
}

void refreshLine(const std::wstring& line, int y) {
    move(y, 0);
    clrtoeol();
    printw("%ls", line.c_str());
    refresh();
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    printw("Welcome to Linux Text Editor Xyliot!\n Version 1.0.r1.b7\nTips1:After enter filename, you can write anythink. Press F1 to save and exit（Your files and programs are in the same directory）\nTips2:If the file name you input already exists, you can modify that file.\nTips3:Although I know this is quite basic, this version can only do it this way. Move the program anywhere to edit files anywhere.\nEnter filename: ");
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
    if (!lines.empty()) {
        for (const auto& line : lines) {
            printw("%ls\n", line.c_str());
        }
    }

    move(y, x);
    refresh();

    wint_t ch;
    while (true) {
        get_wch(&ch);
        if (ch == KEY_F(1)) { // F1 键保存并退出
            break;
        }

        switch (ch) {
            case KEY_BACKSPACE:
            case 127:
                if (x > 0) {
                    lines[y].erase(--x, 1);
                    refreshLine(lines[y], y);
                } else if (y > 0) {
                    x = lines[y - 1].size();
                    lines[y - 1] += lines[y];
                    lines.erase(lines.begin() + y);
                    y--;
                    clear();
                    for (const auto& line : lines) {
                        printw("%ls\n", line.c_str());
                    }
                }
                break;
            case KEY_DC:
                if (x < lines[y].size()) {
                    lines[y].erase(x, 1);
                    refreshLine(lines[y], y);
                }
                break;
            case '\n':
                lines.insert(lines.begin() + y + 1, lines[y].substr(x));
                lines[y] = lines[y].substr(0, x);
                y++;
                x = 0;
                clear();
                for (const auto& line : lines) {
                    printw("%ls\n", line.c_str());
                }
                move(y, x);
                break;
            case KEY_UP:
                if (y > 0) {
                    y--;
                    x = std::min(x, (int)lines[y].size());
                    move(y, x);
                }
                break;
            case KEY_DOWN:
                if (y < lines.size() - 1) {
                    y++;
                    x = std::min(x, (int)lines[y].size());
                    move(y, x);
                }
                break;
            case KEY_LEFT:
                if (x > 0) {
                    x--;
                } else if (y > 0) {
                    y--;
                    x = lines[y].size();
                }
                move(y, x);
                break;
            case KEY_RIGHT:
                if (x < lines[y].size()) {
                    x++;
                } else if (y < lines.size() - 1) {
                    y++;
                    x = 0;
                }
                move(y, x);
                break;
            default:
                if (ch < 128) {
                    lines[y].insert(lines[y].begin() + x, ch);
                    refreshLine(lines[y], y);
                    x++;
                } else {
                    wchar_t wch = ch;
                    lines[y].insert(lines[y].begin() + x, wch);
                    refreshLine(lines[y], y);
                    x++;
                }
                break;
        }
        move(y, x);
        refresh();
    }

    saveFile(filename, lines);

    endwin();

    return 0;
}

