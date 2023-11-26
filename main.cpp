#include<iostream>
#include<string>
#include<conio.h>
#include<windows.h>
using namespace std;

// Prototypes
    void gotoxy(int x, int y);
    void set_screen_sizes();
    void cursor_hide();
    void print_title(int x, int y);
    int get_center(int w);
    void print_on_center(int y, string str);

    int print_rc_options(int x, int y, int w, int option_selected);
    
    void set_rc();

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

// Keys

    int key_space = 32;
    int key_up = 72;
    int key_down = 80;
    int key_left = 75;
    int key_right = 77;

// States
int screen_width, screen_height;
int rows = 3, columns = 3; // Min : 3x3
int grid[8][8]; // Max : 8x8
int line_number = 10;

main()
{
    cursor_hide();
    set_rc();
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

void cursor_hide()
{
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

int get_center(int w)
{
    int center = (screen_width - w)/2;
    if (center < 0) center = 0;
    return center;
}

void print_on_center(int y, string str)
{
    int string_length = str.length();
    int center_x = (screen_width - string_length)/2;
    if (string_length >= screen_width) center_x = 0;
    gotoxy(center_x, y);
    cout << str;
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

void set_rc()
{
    system("cls");
    set_screen_sizes();
    cout << bright_magenta;
    print_title(get_center(70),2);

    int center = get_center(30);

    cout << bright_yellow;
    print_on_center(++line_number, "Press Space Key To Get Started With The Game !");

    line_number += 2;
    gotoxy(center, line_number);
    cout << bright_cyan << "Select Number of Columns :";

    line_number += 2;
    columns = print_rc_options(center, line_number, 30, 0);

    
    line_number += 3;
    gotoxy(center, line_number);
    cout << bright_cyan << "Select Number of Rows :";

    line_number += 2;
    rows = print_rc_options(center, line_number, 30, 0);

    string empty_row = "";
    for (int i = 0; i < screen_width; i++) empty_row += ' ';
    
    for (int i = 0; i < 11; i++)
    {
        gotoxy(0, 10 + i);
        cout << empty_row;
    }
}

int print_rc_options(int x, int y, int w, int option_selected)
{
    int options[] = {3,4,5,6,7,8};
    int options_length = 6;

    int gap = w/options_length;

    int input = 0;

    do {

        if (input != 0)
        {
            input = getch();
            if (!(input == key_left || input == key_right)) continue;
        }

             if (input == key_left) option_selected = (option_selected <= 0 ? options_length - 1 : option_selected - 1);
        else if (input == key_right) option_selected = (option_selected >= options_length - 1 ? 0 : option_selected + 1);

        for (int i = 0; i < options_length; i++)
        {
            int x_point = x + (i*gap);
            gotoxy(x_point, y);
            cout << (option_selected == i ? bright_yellow : white);
            cout << options[i];
            gotoxy(x_point, y+1);
            cout << bright_yellow << (option_selected == i ? "^" : " ");
        }


        if (input == 0) input++;

    } while (input != key_space);

    gotoxy(x+(option_selected*gap), y+1); cout << " ";
    return options[option_selected];
}