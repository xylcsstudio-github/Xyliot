Xyliot 1.0.r1.b8
By LosefDevelopLab From XYLCS Studio.

(en-us)Install steps：
1.bash install dependencies
Debian/Ubuntu:
sudo apt install libncurses5-dev
sudo apt install g++ make

Fedora:
sudo dnf install ncurses-devel
sudo dnf install g++ make

CentOS/RHEL:
sudo yum install ncurses-devel
sudo yum install g++ make

MacOS:Install Xcode command line tools and ncurses via Homebrew:
xcode-select --install
brew install ncurses


2.enter bash to installation(more precisely, compilation) and start Xyliot(Before that, please cd to the directory where Xyliot is located.)：
g++ -o xyliot xyliot.cpp -lncursesw
./xyliot



Tips1:After enter filename, you can write anythink. Press F1 to save and exit（Your files and programs are in the same directory）.
Tips2:If the file name you input already exists, you can modify that file.
Tips3:Although I know this is quite basic, this version can only do it this way. Move the program anywhere to edit files anywhere.
Tip 4: PgUP and PgDn can move to the top and bottom.


(zh-cn)安装步骤：
1. 在终端中执行以下命令安装依赖：

Debian/Ubuntu:
sudo apt install libncurses5-dev
sudo apt install g++ make

Fedora:
sudo dnf install ncurses-devel
sudo dnf install g++ make

CentOS/RHEL:
sudo yum install ncurses-devel
sudo yum install g++ make

MacOS:
首先安装Xcode命令行工具，然后通过Homebrew安装ncurses：
xcode-select --install
brew install ncurses

2. 进入终端，使用以下命令安装（更确切的来说是编译）启动Xyliot（在此之前，请cd到Xyliot所在的目录）：
g++ -o xyliot xyliot.cpp -lncursesw
./xyliot

提示1：在输入文件名后，您可以写任何内容。按F1保存并退出（您的文件和程序位于同一目录中）。
提示2：如果您输入的文件名已存在，您可以修改该文件。
提示3：尽管我知道这很基础，但这个版本只能以这种方式完成。将程序移动到任何位置以编辑任何文件。
提示4：PgUP和PgDn可以移动到顶端和底端
