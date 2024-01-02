#include<iostream>
#include<string>
#include<conio.h>
#include<windows.h>
#include<ctime>
#include<random>
#include<vector>
#include<algorithm>

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
    void generate_grid();
    int get_random_number(int start, int end);
    void fill_grid();
    string remove_character(string input_str, char char_to_remove);
    void handle_grid_movement();
    void shuffle_array(int arr[], int size);
    string get_grid_fill_sequence();
    bool check_for_victory();

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
    int key_escpae = 27;

// States
int screen_width, screen_height;
int rows = 3, columns = 3; // Min : 3x3
char grid[8][8]; // Max : 8x8
vector<int> cursor = {0,0}; // x,y
vector<int> selected = {-1,-1}; // x,y
int line_number = 10;
int attempts = 0;

main()
{
    srand(time(0));
    cursor_hide();
    set_rc();
    generate_grid();
    handle_grid_movement();
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

void generate_box(int x, int y)
{
    gotoxy(x, y+0); cout << "-----";
    gotoxy(x, y+1); cout << "|   |";
    gotoxy(x, y+2); cout << "-----";
}

void remove_box(int x, int y)
{
    gotoxy(x, y+0); cout << "     ";
    gotoxy(x, y+1); cout << "     ";
    gotoxy(x, y+2); cout << "     ";
}

void generate_grid()
{
    int screen_x = get_center(columns*6);
    int screen_y = (screen_height - 10 - rows*4)/2;

    fill_grid();

    cout << white;
    for (int y = 0; y < rows; y++)
        for (int x = 0; x < columns; x++)
        {
            if (y == cursor[1] && x == cursor[0])
                cout << bright_magenta;
            else if (grid[y][x] == '#')
                cout << black;
            else if (x == selected[0] && y == selected[1])
                generate_highlighted_box(y,x);
            else
                cout << bright_white;
            generate_box(screen_x+(x*7),10+screen_y+(y*4));
        }
    
}

int get_random_number(int start, int end) {

    // Generate a random number between start and end (inclusive)
    int random_number = std::rand() % (end - start + 1) + start;

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

void shuffle_array(int arr[], int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    shuffle(arr, arr + size, gen);
}

string get_grid_fill_sequence() {
    // The grid needs to be filled so that only rows * columns / 2 different characters are selected
    // Then Doubled
    // And placed randomly all over the grid
    // And if the product is even then one must be whitespace
    string random_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";

    int unique_chars_needed = (rows * columns / 2 == 0) ? rows*columns/2 : rows*columns/2 + 1;

    // Create a string with unique characters
    string unique_chars(random_chars.begin(), random_chars.begin() + unique_chars_needed);
    unique_chars += unique_chars;

    mt19937 g(time(0));
    shuffle(unique_chars.begin(), unique_chars.end(), g);

    // Create the resultant string
    string resultant;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            resultant += unique_chars[i * columns + j];
        }
    }

    // Add a whitespace if the product is odd
    if ((rows * columns) % 2 != 0) {
        resultant[rows * columns / 2] = ' ';
    }

    return resultant;
}
string remove_character(string input_str, char char_to_remove) {
    string result_str;
    for (char c : input_str) {
        if (c != char_to_remove) {
            result_str += c;
        }
    }
    return result_str;
}

void generate_highlighted_box(int box_row, int box_column)
{
    cout << bright_cyan;
    generate_box(box_column+(cursor[0]*7),10+box_row+(cursor[1]*4));
    gotoxy(box_column+(cursor[0]*7)+2,10+box_row+(cursor[1]*4)+1);
    cout << grid[cursor[1]][cursor[0]];
}

void erase_box(int box_row, int box_column)
{
    remove_box(box_column+(cursor[0]*7),10+box_row+(cursor[1]*4));
}

bool cell_is_available(int row, int column)
{
    bool cell_is_null = grid[row][column] == '#';
    bool cell_has_already_been_selected = (column == selected[0] && row == selected[1]);
    return (!cell_is_null && !cell_has_already_been_selected);
}

vector<int> calculate_upward_movement()
{
    int x = cursor[0], y = cursor[1];
    for (int i = 1; i < rows; i++)
    {
        int new_y = y - i;
        if (new_y < 0)
            new_y += rows;
        if (cell_is_available(new_y, x))
            return {x,new_y};
    }
    return {x,y};
}

vector<int> calculate_leftward_movement()
{
    int x = cursor[0], y = cursor[1];
    for (int i = 1; i < columns; i++)
    {
        int new_x = x - i;
        if (new_x < 0)
            new_x += columns;
        if (cell_is_available(y, new_x))
            return {new_x,y};
    }
    return {x,y};
}

vector<int> calculate_downward_movement()
{
    int x = cursor[0], y = cursor[1];
    for (int i = 1; i < rows; i++)
    {
        int new_y = y + i;
        if (new_y >= rows)
            new_y -= rows;
        if (cell_is_available(new_y, x))
            return {x,new_y};
    }
    return {x,y};
}

vector<int> calculate_rightward_movement()
{
    int x = cursor[0], y = cursor[1];
    for (int i = 1; i < columns; i++)
    {
        int new_x = x + i;
        if (new_x >= columns)
            new_x -= columns;
        if (cell_is_available(y, new_x))
            return {new_x,y};
    }
    return {x,y};
}

void move_to_near_cell()
{
    int x = get_center(columns*6);
    int y = (screen_height - 10 - rows*4)/2;

    vector<int> coordinates = calculate_rightward_movement();
    if (coordinates[0] == cursor[0] && coordinates[1] == cursor[1])
        coordinates = calculate_downward_movement();
    if (coordinates[0] == cursor[0] && coordinates[1] == cursor[1])
        check_for_victory();
    // Then highlight that box
    cursor[0] = coordinates[0];
    cursor[1] = coordinates[1];
    cout << bright_magenta;
    generate_box(x+(cursor[0]*7),10+y+(cursor[1]*4));
}

void handle_grid_movement()
{

    int x = get_center(columns*6);
    int y = (screen_height - 10 - rows*4)/2;

    while (true) // Game Loop
    {
        gotoxy(0,0); cout << cursor[0] << "," << cursor[1];

        int c = getch();
        vector<int> new_cordinates;

        if (c == key_up || c == key_down || c == key_left || c == key_right)
        {
            if (cursor[1] != selected[1] || cursor[0] != selected[0])
            {
                cout << bright_white;
                generate_box(x+(cursor[0]*7),10+y+(cursor[1]*4));
            }
        }
        
        if (c == key_up)
        {
            new_cordinates = calculate_upward_movement();
        }
        else if (c == key_down)
        {
            new_cordinates = calculate_downward_movement();
        }
        else if (c == key_left)
        {
            new_cordinates = calculate_leftward_movement();
        }
        else if (c == key_right)
        {
            new_cordinates = calculate_rightward_movement();
        }
        if (c == key_up || c == key_down || c == key_left || c == key_right)
        {
            cursor[0] = new_cordinates[0];
            cursor[1] = new_cordinates[1];
            cout << bright_magenta;
            generate_box(x+(cursor[0]*7),10+y+(cursor[1]*4));
        }
        
        // Process the game
        if (c == key_space)
        {
            generate_highlighted_box(y,x);

            // In case cell is just whitespace, skip it
            if (grid[cursor[1]][cursor[0]] == ' ')
            {
                Sleep(500);
                grid[cursor[1]][cursor[0]] = '#';
                erase_box(y,x);
                move_to_near_cell();
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
                    remove_box(x+(selected[0]*7),10+y+(selected[1]*4));
                    remove_box(x+(cursor[0]*7),10+y+(cursor[1]*4));
                    selected[0] = -1; selected[1] = -1;
                    move_to_near_cell();
                }
                else // Otherwise, just reset
                {
                    cout << bright_white;
                    generate_box(x+(cursor[0]*7),10+y+(cursor[1]*4));
                    generate_box(x+(selected[0]*7),10+y+(selected[1]*4));            
                    selected[0] = -1, selected[1] = -1;
                }
            }
            
        }
        else if (c == key_escpae)
        {
            exit(0);
        }

    }
}

bool check_for_victory()
{
    // Check for victory
    int nulls = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            if (grid[i][j] == '#') nulls++;
    if (nulls == rows*columns)
    {
        system("cls");
        gotoxy(0,0); cout << "YOU WIN !";
        return 1;
    }
    return 0;
}