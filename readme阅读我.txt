Xyliot 1.0.r1.b12
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
Tips3:PgUP and PgDn can move to the top and bottom.
Tips4:You can input either absolute or relative file paths when prompted.
Tips5:Move the compiled program to /usr/local/bin so that you can run it from anywhere just by typing its name next time!

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
提示3：PgUP和PgDn可以移动到顶端和底端
提示4：输入文件路径时可输入绝对路径或相对路径
提示5：将编译出来的程序移动到/usr/local/bin，下次无论在什么路径都可以只输入这个程序的名字就可以运行啦
