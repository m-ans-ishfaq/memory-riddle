#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

// Prototypes
    void gotoxy(int x, int y);
    void set_screen_sizes();
    void print_title(int x, int y);
    int get_center(int w);

// Colors
    string black = "\033[30m";
    string red = "\033[31m";
    string green = "\033[32m";
    string yellow = "\033[33m";
    string blue = "\033[34m";
    string magenta = "\033[35m";
    string cyan = "\033[36m";
    string white = "\033[37m";
    string bright_black = "\033[90m";
    string bright_red = "\033[91m";
    string bright_green = "\033[92m";
    string bright_yellow = "\033[93m";
    string bright_blue = "\033[94m";
    string bright_magenta = "\033[95m";
    string bright_cyan = "\033[96m";
    string bright_white = "\033[97m";

// States
int screen_width, screen_height;
int rows = 3, columns = 3;
int grid[10][10];

main()
{
    system("cls");
    set_screen_sizes();
    cout << bright_magenta;
    print_title(get_center(70),2);
}

void gotoxy(int x, int y)
{
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

void set_screen_sizes()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int get_center(int w)
{
    int center = (screen_width - w)/2;
    if (center < 0) center = 0;
    return center;
}

void print_title(int x, int y)
{
    string title[] = {
        "___  ___                                 ______ _     _     _ _      ", 
        "|  \\/  |                                 | ___ (_)   | |   | | |     ", 
        "| .  . | ___ _ __ ___   ___  _ __ _   _  | |_/ /_  __| | __| | | ___ ",
        "| |\\/| |/ _ \\ '_ ` _ \\ / _ \\| '__| | | | |    /| |/ _` |/ _` | |/ _ \\",
        "| |  | |  __/ | | | | | (_) | |  | |_| | | |\\ \\| | (_| | (_| | |  __/",
        "\\_|  |_/\\___|_| |_| |_|\\___/|_|   \\__, | \\_| \\_|_|\\__,_|\\__,_|_|\\___|",
        "                                   __/ |                            ", 
        "                                  |___/                            "  
    };

    for (int i = 0; i < 8; i++)
    {
        gotoxy(x,y+i);
        cout << title[i];
    }

}