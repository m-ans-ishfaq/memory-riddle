#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
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
void generate_box(int x, int y);
void generate_highlighted_box(int box_row, int box_column);
void generate_grid();
int get_random_number(int start, int end);
void fill_grid();
string remove_character(string input_str, char char_to_remove);
void print_attempts_bar();
void handle_grid_movement();
void shuffle_array(int arr[], int size);
string get_grid_fill_sequence();
bool check_for_victory();
void you_lose();

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
int key_escape = 27;

// States
int screen_width, screen_height;
int rows = 3, columns = 3;       // Min : 3x3
char grid[8][8];                 // Max : 8x8
vector<int> cursor = {0, 0};     // x,y
vector<int> selected = {-1, -1}; // x,y
int line_number = 10;
int total_attempts;
int attempts = 0;

main()
{
    srand(time(0));
    cursor_hide();
    set_rc(); // Set row and column
    generate_grid();
    total_attempts = rows * columns;
    print_attempts_bar();   // Initialized
    handle_grid_movement(); // Game Loop
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
    int center = (screen_width - w) / 2;
    if (center < 0)
        center = 0;
    return center;
}

void print_on_center(int y, string str)
{
    int string_length = str.length();
    int center_x = (screen_width - string_length) / 2;
    if (string_length >= screen_width)
        center_x = 0;
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
        "                                  |___/                            "};

    for (int i = 0; i < 8; i++)
    {
        gotoxy(x, y + i);
        cout << title[i];
    }
}

void set_rc()
{
    system("cls");
    set_screen_sizes();
    cout << bright_magenta;
    print_title(get_center(70), 2);

    int center = get_center(30);
    int y = line_number;

    cout << bright_yellow;
    print_on_center(++y, "Press Space Key To Get Started With The Game !");

    y += 2;
    gotoxy(center, y);
    cout << bright_cyan << "Select Number of Columns :";

    y += 2;
    columns = print_rc_options(center, y, 30, 0);

    y += 3;
    gotoxy(center, y);
    cout << bright_cyan << "Select Number of Rows :";

    y += 2;
    rows = print_rc_options(center, y, 30, 0);

    string empty_row = "";
    for (int i = 0; i < screen_width; i++)
        empty_row += ' ';

    for (int i = 0; i < 11; i++)
    {
        gotoxy(0, 10 + i);
        cout << empty_row;
    }
}

int print_rc_options(int x, int y, int w, int option_selected)
{
    int options[] = {3, 4, 5, 6, 7, 8};
    int options_length = 6;
    int gap = w / options_length;
    int input = 0;

    do {
        if (input != 0)
        {
            input = getch();
            if (!(input == key_left || input == key_right))
                continue;
        }

        if (input == key_left)
            option_selected = (option_selected <= 0 ? options_length - 1 : option_selected - 1);
        else if (input == key_right)
            option_selected = (option_selected >= options_length - 1 ? 0 : option_selected + 1);

        for (int i = 0; i < options_length; i++)
        {
            int x_point = x + (i * gap);
            gotoxy(x_point, y);
            cout << (option_selected == i ? bright_yellow : white);
            cout << options[i];
            gotoxy(x_point, y + 1);
            cout << bright_yellow << (option_selected == i ? "^" : " ");
        }

        if (input == 0)
            input++;

    } while (input != key_space);

    gotoxy(x + (option_selected * gap), y + 1);
    cout << " ";
    return options[option_selected];
}

void generate_box(int x, int y)
{
    gotoxy(x, y + 0);
    cout << "-----";
    gotoxy(x, y + 1);
    cout << "|   |";
    gotoxy(x, y + 2);
    cout << "-----";
}

void remove_box(int x, int y)
{
    gotoxy(x, y + 0);
    cout << "     ";
    gotoxy(x, y + 1);
    cout << "     ";
    gotoxy(x, y + 2);
    cout << "     ";
}

void generate_grid()
{
    int screen_x = get_center(columns * 6);
    int screen_y = (screen_height - 10 - rows * 4) / 2;

    fill_grid();

    cout << white;
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            if (y == cursor[1] && x == cursor[0])
                cout << bright_magenta;
            else if (grid[y][x] == '#')
                cout << black;
            else if (x == selected[0] && y == selected[1])
                generate_highlighted_box(y, x);
            else
                cout << bright_white;
            generate_box(screen_x + (x * 7), 10 + screen_y + (y * 4));
            gotoxy(screen_x + (x * 7) + 2, 10 + screen_y + (y * 4) + 1);
            cout << grid[y][x];
        }
    }
}

int get_random_number(int start, int end)
{
    int random_number = rand() % (end - start + 1) + start;
    return random_number;
}

void fill_grid()
{

    string grid_fill_sequence = get_grid_fill_sequence();
    int row = 0, column = 0;

    for (int i = 0; i < grid_fill_sequence.length(); i++)
    {
        grid[row][column] = grid_fill_sequence[i];
        column++;
        if (column >= columns)
        {
            column = 0;
            row++;
        }
    }
}

void shuffle_array(int arr[], int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    shuffle(arr, arr + size, gen);
}

string get_grid_fill_sequence()
{
    /*
     * The grid needs to be filled so that only rows * columns / 2 different
     * characters are selected then doubled and placed randomly all over the grid.
     * If the product is even then one must be whitespace to balance the grid.
     */

    string random_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";

    int unique_chars_needed = rows * columns / 2;
    string sequence = "";
    for (int i = 0; i < unique_chars_needed; i++)
    {
        int random_chars_length = random_chars.length();
        while (true)
        {
            int random_index = rand() % random_chars_length;
            char random_char = random_chars[random_index];
            for (int j = 0; j < sequence.length(); j++)
                if (sequence[j] == random_char)
                    continue;
            sequence += random_char;
            break;
        }
    }
    sequence += sequence;
    if (rows * columns / 2 % 2 != 0)
        sequence += ' ';
    
    // Bogo Sorting
    mt19937 g(time(0));
    shuffle(sequence.begin(), sequence.end(), g);

    return sequence;
}
string remove_character(string input_str, char char_to_remove)
{
    string result_str;
    for (char c : input_str)
        if (c != char_to_remove)
            result_str += c;

    return result_str;
}

void generate_highlighted_box(int box_row, int box_column)
{
    cout << bright_cyan;
    generate_box(box_column + (cursor[0] * 7), 10 + box_row + (cursor[1] * 4));
    gotoxy(box_column + (cursor[0] * 7) + 2, 10 + box_row + (cursor[1] * 4) + 1);
    cout << grid[cursor[1]][cursor[0]];
}

void erase_box(int box_row, int box_column)
{
    remove_box(box_column + (cursor[0] * 7), 10 + box_row + (cursor[1] * 4));
}

bool cell_is_available(int row, int column)
{
    bool cell_is_null = grid[row][column] == '#';
    bool cell_has_already_been_selected = (column == selected[0] && row == selected[1]);
    return (!cell_is_null && !cell_has_already_been_selected);
}

void print_attempts_bar()
{
    int title_width = 70;
    int attempts_left = total_attempts - attempts;
    float attempts_left_percent = attempts_left / float(total_attempts);
    int highlighted_bar_length = attempts_left_percent * title_width;
    int null_bar_length = title_width - highlighted_bar_length;
    string bar = "";
    int percentage = attempts_left_percent * 100;
    if (percentage > 66)
        bar += bright_green;
    else if (percentage > 33)
        bar += bright_yellow;
    else
        bar += bright_red;
    for (int i = 0; i < highlighted_bar_length; i++)
        bar += '\xDC';
    bar += bright_black;
    for (int i = 0; i < null_bar_length; i++)
        bar += '\xDC';
    gotoxy((screen_width - title_width) / 2, 0);
    cout << bar;
}

void handle_grid_movement()
{

    int x = get_center(columns * 6);
    int y = (screen_height - 10 - rows * 4) / 2;

    while (true) // Game Loop
    {
        int c = getch();
        bool pressed_arrow_key = c == key_up || c == key_down || c == key_left || c == key_right;

        // On any arrow key press
        if (pressed_arrow_key)
        {
            if (cursor[1] != selected[1] || cursor[0] != selected[0])
            {
                if (grid[cursor[1]][cursor[0]] == '#')
                    cout << black;
                else
                    cout << bright_white;
                generate_box(x + (cursor[0] * 7), 10 + y + (cursor[1] * 4));
            }
        }

        // What to do on each arrow key press
        if (c == key_up)
            cursor[1] = (cursor[1] <= 0 ? rows - 1 : cursor[1] - 1);
        else if (c == key_down)
            cursor[1] = (cursor[1] >= rows - 1 ? 0 : cursor[1] + 1);
        else if (c == key_left)
            cursor[0] = (cursor[0] <= 0 ? columns - 1 : cursor[0] - 1);
        else if (c == key_right)
            cursor[0] = (cursor[0] >= columns - 1 ? 0 : cursor[0] + 1);

        // After this (arrow key press)
        if (pressed_arrow_key)
        {
            if (grid[cursor[1]][cursor[0]] == '#')
                cout << bright_black;
            else
                cout << bright_magenta;

            generate_box(x + (cursor[0] * 7), 10 + y + (cursor[1] * 4));
        }

        // Processing the game on space press
        if (c == key_space)
        {
            if (grid[cursor[1]][cursor[0]] == '#')
                continue;

            generate_highlighted_box(y, x);

            // In case cell is just whitespace, skip it
            if (grid[cursor[1]][cursor[0]] == ' ')
            {
                Sleep(500);
                grid[cursor[1]][cursor[0]] = '#';
                erase_box(y, x);
                check_for_victory();
                continue;
            }

            if (selected[1] == -1)
            {
                selected[1] = cursor[1];
                selected[0] = cursor[0];
            }
            else
            {
                Sleep(500);
                // If same
                if (grid[selected[1]][selected[0]] == grid[cursor[1]][cursor[0]] && !(selected[1] == cursor[1] && selected[0] == cursor[0]))
                {
                    grid[selected[1]][selected[0]] = '#';
                    grid[cursor[1]][cursor[0]] = '#';
                    remove_box(x + (selected[0] * 7), 10 + y + (selected[1] * 4));
                    remove_box(x + (cursor[0] * 7), 10 + y + (cursor[1] * 4));
                    selected[0] = -1;
                    selected[1] = -1;
                }
                else // Otherwise, just reset
                {
                    cout << bright_white;
                    generate_box(x + (cursor[0] * 7), 10 + y + (cursor[1] * 4));
                    generate_box(x + (selected[0] * 7), 10 + y + (selected[1] * 4));
                    selected[0] = -1, selected[1] = -1;
                    attempts++;

                    if (attempts >= total_attempts)
                        you_lose();

                    print_attempts_bar();
                }
                check_for_victory();
            }
        }
        // Escape key for exitting game loop
        else if (c == key_escape)
        {
            exit(0);
        }
    }
}

void you_lose()
{
    string bars = bright_red;
    for (int i = 0; i < 70; i++)
        bars += '\xDC';

    gotoxy((screen_width - 70) / 2, (screen_height - 5) / 2);
    cout << bars;
    print_on_center((screen_height - 5) / 2 + 2, "YOU LOSE!");
    gotoxy((screen_width - 70) / 2, (screen_height - 5) / 2 + 4);
    cout << bars;
    getch();
    system("cls");
    exit(0);
}

bool check_for_victory()
{
    // Check for victory
    int nulls = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            if (grid[i][j] == '#')
                nulls++;

    if (nulls == rows * columns)
    {
        string bars = bright_cyan;
        for (int i = 0; i < 70; i++)
            bars += '\xDC';

        gotoxy((screen_width - 70) / 2, (screen_height - 5) / 2);
        cout << bars;
        print_on_center((screen_height - 5) / 2 + 2, "CONGRATULATIONS! YOU WIN!");
        gotoxy((screen_width - 70) / 2, (screen_height - 5) / 2 + 4);
        cout << bars;
        getch();
        system("cls");
        exit(0);
    }

    return 0;
}