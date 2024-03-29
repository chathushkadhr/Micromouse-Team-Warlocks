#define DIM 14
#define TOTAL DIM *DIM + 1
#define CENTER DIM *(DIM - 1) / 2

#define DESTINATION CENTER // Up left corner cell of the destination

#define PUSH_BUTTON1_PIN 13 // LEFT SEARCH INT
#define PUSH_BUTTON2_PIN 14 // RIGHT FAST

#define DEBOUNCE_DELAY 500

/* dir:
 1 - east
 2 - west
 3 - south
 4 - north

 6 - left
 7 - right
 8 - back

 -1 - forward */

#define DIR_EAST 1
#define DIR_WEST 2
#define DIR_SOUTH 3
#define DIR_NORTH 4

#define DIR_LEFT 6
#define DIR_RIGHT 7
#define DIR_BACK 8

#define DIR_FORWARD -1

bool commingBack = false;

// int walls[TOTAL][4] =  {{0, 0, 0, 0},{2, 0, 0, 0},{1, 3, 0, 0},{2, 0, 0, 0},{10, 0, 0, 0},{11, 0, 0, 0},{0, 0, 0, 0},{8, 0, 0, 0},{7, 0, 0, 0},{15, 0, 0, 0},{16, 4, 0, 0},{5, 0, 0, 0},{0, 0, 0, 0},{19, 0, 0, 0},{15, 0, 0, 0},{14, 9, 0, 0},{10, 17, 0, 0},{16, 18, 23, 0},{17, 0, 0, 0},{13, 0, 0, 0},{21, 0, 0, 0},{20, 27, 0, 0},{28, 0, 0, 0},{29, 17, 0, 0},{0, 0, 0, 0},{26,0, 0, 0},{25, 27, 0, 0},{26, 21, 0, 0},{22, 34, 0, 0},{23, 30, 0, 0},{29, 0, 0, 0},{32, 0, 0, 0},{31, 0, 0, 0},{0,0, 0, 0},{28, 35, 0, 0},{34, 0, 0, 0},{0, 0, 0, 0}};
// int walls[TOTAL][4] = {{0, 0, 0, 0}, {2, 0, 0, 0}, {1, 3, 0, 0}, {2, 0, 0, 0}, {5, 0, 0, 0}, {4, 6, 0, 0}, {5, 0, 0, 0}, {8, 0, 0, 0}, {7, 0, 0, 0}, {15, 10, 0, 0}, {16, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {19, 0, 0, 0}, {15, 0, 0, 0}, {9, 21, 14, 16}, {10, 22, 15, 0}, {18, 0, 0, 0}, {24, 17, 0, 0}, {13, 0, 0, 0}, {26, 0, 0, 0}, {15, 22, 0, 0}, {16, 21, 0, 0}, {24, 0, 0, 0}, {18, 23, 0, 0}, {26, 0, 0, 0}, {20, 25, 27, 0}, {26, 28, 0, 0}, {34, 27, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {28, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
int walls[TOTAL][4] =
    {{0, 0, 0, 0}, {15, 0, 0, 0}, {0, 0, 0, 0}, {17, 0, 0, 0}, {18, 0, 0, 0}, {0, 0, 0, 0}, {20, 0, 0, 0}, {0, 0, 0, 0}, {22, 0, 0, 0}, {23, 10, 0, 0}, {24, 9, 0, 0}, {25, 0, 0, 0}, {26, 0, 0, 0}, {27, 0, 0, 0}, {0, 0, 0, 0}, {1, 29, 0, 0}, {30, 0, 0, 0}, {3, 31, 0, 0}, {4, 19, 0, 0}, {18, 0, 0, 0}, {6, 34, 21, 0}, {20, 0, 0, 0}, {8, 36, 0, 0}, {9, 24, 0, 0}, {10, 38, 23, 0}, {11, 39, 0, 0}, {12, 0, 0, 0}, {13, 28, 0, 0}, {27, 0, 0, 0}, {15, 43, 0, 0}, {16, 44, 0, 0}, {17, 32, 0, 0}, {31, 33, 0, 0}, {32, 0, 0, 0}, {20, 48, 0, 0}, {49, 36, 0, 0}, {22, 35, 37, 0}, {36, 38, 0, 0}, {24, 52, 37, 0}, {25, 40, 0, 0}, {39, 41, 0, 0}, {55, 40, 42, 0}, {41, 0, 0, 0}, {29, 57, 0, 0}, {30, 45, 0, 0}, {44, 46, 0, 0}, {45, 47, 0, 0}, {46, 0, 0, 0}, {34, 0, 0, 0}, {35, 63, 0, 0}, {64, 51, 0, 0}, {50, 52, 0, 0}, {38, 51, 53, 0}, {67, 52, 0, 0}, {68, 0, 0, 0}, {41, 0, 0, 0}, {70, 0, 0, 0}, {43, 58, 0, 0}, {57, 59, 0, 0}, {58, 0, 0, 0}, {0, 0, 0, 0}, {75, 62, 0, 0}, {61, 63, 0, 0}, {49, 77, 62, 0}, {50, 78, 0, 0}, {79, 66, 0, 0}, {80, 65, 0, 0}, {53, 81, 0, 0}, {54, 82, 0, 0}, {83, 0, 0, 0}, {56, 0, 0, 0}, {72, 0, 0, 0}, {71, 0, 0, 0}, {87, 74, 0, 0}, {73, 75, 0, 0}, {61, 74, 0, 0}, {90, 0, 0, 0}, {63, 91, 0, 0}, {64, 92, 0, 0}, {65, 80, 0, 0}, {66, 94, 79, 0}, {67, 0, 0, 0}, {68, 96, 0, 0}, {69, 0, 0, 0}, {98, 0, 0, 0}, {99, 86, 0, 0}, {85, 87, 0, 0}, {73, 86, 0, 0}, {102, 89, 0, 0}, {88, 90, 0, 0}, {76, 89, 0, 0}, {77, 0, 0, 0}, {78, 93, 0, 0}, {92, 94, 0, 0}, {80, 108, 93, 0}, {0, 0, 0, 0}, {82, 110, 97, 0}, {96, 0, 0, 0}, {84, 112, 0, 0}, {85, 0, 0, 0}, {114, 0, 0, 0}, {115, 0, 0, 0}, {88, 0, 0, 0}, {117, 104, 0, 0}, {103, 105, 0, 0}, {119, 104, 0, 0}, {120, 107, 0, 0}, {106, 108, 0, 0}, {94, 107, 0, 0}, {123, 0, 0, 0}, {96, 0, 0, 0}, {0, 0, 0, 0}, {98, 126, 0, 0}, {127, 0, 0, 0}, {100, 128, 0, 0}, {101, 129, 116, 0}, {115, 117, 0, 0}, {103, 116, 0, 0}, {132, 0, 0, 0}, {105, 133, 0, 0}, {106, 134, 0, 0}, {135, 0, 0, 0}, {123, 0, 0, 0}, {109, 122, 124, 0}, {138, 123, 125, 0}, {124, 126, 0, 0}, {112, 140, 125, 0}, {113, 0, 0, 0}, {114, 142, 0, 0}, {115, 143, 0, 0}, {144, 131, 0, 0}, {130, 0, 0, 0}, {118, 146, 0, 0}, {119, 0, 0, 0}, {120, 0, 0, 0}, {121, 136, 0, 0}, {135, 0, 0, 0}, {0, 0, 0, 0}, {124, 152, 0, 0}, {153, 0, 0, 0}, {126, 154, 0, 0}, {0, 0, 0, 0}, {128, 143, 0, 0}, {129, 142, 144, 0}, {130, 158, 143, 0}, {146, 0, 0, 0}, {132, 145, 0, 0}, {161, 148, 0, 0}, {147, 149, 0, 0}, {148, 150, 0, 0}, {164, 149, 151, 0}, {150, 0, 0, 0}, {138, 166, 153, 0}, {139, 152, 0, 0}, {140, 168, 0, 0}, {156, 0, 0, 0}, {155, 157, 0, 0}, {156, 0, 0, 0}, {144, 172, 0, 0}, {160, 0, 0, 0}, {159, 0, 0, 0}, {147, 175, 0, 0}, {176, 163, 0, 0}, {162, 164, 0, 0}, {150, 163, 0, 0}, {0, 0, 0, 0}, {152, 167, 0, 0}, {166, 168, 0, 0}, {154, 167, 0, 0}, {170, 0, 0, 0}, {184, 169, 171, 0}, {170, 172, 0, 0}, {158, 171, 0, 0}, {187, 174, 0, 0}, {188, 173, 0, 0}, {161, 189, 0, 0}, {162, 190, 0, 0}, {191, 0, 0, 0}, {179, 0, 0, 0}, {178, 180, 0, 0}, {194, 179, 0, 0}, {195, 182, 0, 0}, {181, 0, 0, 0}, {184, 0, 0, 0}, {170, 183, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {173, 0, 0, 0}, {174, 0, 0, 0}, {175, 0, 0, 0}, {176, 0, 0, 0}, {177, 0, 0, 0}, {193, 0, 0, 0}, {192, 0, 0, 0}, {180, 0, 0, 0}, {181, 0, 0, 0}, {0, 0, 0, 0}};
// int maze[TOTAL][4];
int final_maze[TOTAL][4];  // CSA
int tmp_maze_forward[TOTAL][4];  // CSA
int tmp_maze_backward[TOTAL][4];  // CSA
bool PUSH_BUTTON1 = false; // CSA
bool PUSH_BUTTON2 = false; // CSA

typedef struct Cell
{
    int cell_number;
    int dir;
    int maze_distance;
} cell;

cell neighbour;

cell neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int neighbours_length;

cell available_neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int available_neighbours_length;

cell min_neighbours[4] = {{0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}, {0, 0, TOTAL}};
int min_neighbours_length;

int starting_cell = 1;
int starting_dir = 3;

int current_cell;
int current_dir;

int next_cell;
int next_dir;

int coordinates[2];
int temp_coordinates[2];

int commands[2 * TOTAL];
int commands_length;

// int md[DIM][DIM] = {{4,3,2,2,3,4},
//       {3,2,1,1,2,3,},
//       {2,1,0,0,1,2},
//       {2,1,0,0,1,2},
//       {3,2,1,1,2,3},
//       {4,3,2,2,3,4}};

int md[DIM][DIM];

int rev_md[DIM][DIM];

int queue[10000];
int queue_start = 0;
int queue_end = 0;
int q_size = 0;

int index_ = 0;
int path[10000];
int shortest_path[TOTAL];
int filtered_path[TOTAL];
int fast_run_commands[TOTAL];
int shortest_path_size = TOTAL;
int filtered_path_size = 0;
int fast_run_commands_size = 0;

void push_button1_isr()
{
    PUSH_BUTTON1 = true;
}

void push_button2_isr()
{
    PUSH_BUTTON2 = true;
}

void fill_md(int up_left_cell)
{

    get_coordinates(up_left_cell);
    int i0 = coordinates[0] - 1;
    int j0 = coordinates[1] - 1;

    for (int i = 0; i < DIM; i++)
    {
        int x;
        if (i > i0)
            x = i - i0 - 1;
        else
            x = i0 - i;

        for (int j = 0; j < DIM; j++)
        {
            int y;
            if (j > j0)
                y = j - j0 - 1;
            else
                y = j0 - j;

            md[i][j] = x + y;
        }
    }

    i0 = -1;
    j0 = -1;

    for (int i = 0; i < DIM; i++)
    {
        int x;
        if (i > i0)
            x = i - i0 - 1;
        else
            x = i0 - i;

        for (int j = 0; j < DIM; j++)
        {
            int y;
            if (j > j0)
                y = j - j0 - 1;
            else
                y = j0 - j;

            rev_md[i][j] = x + y;
        }
    }
}

void print_neighbours(int sel)
{
    int len;
    if (sel == 1)
    {
        len = neighbours_length;
        for (int i = 0; i < len; i++)
        {
        }
    }
    else if (sel == 2)
    {
        len = available_neighbours_length;
        for (int i = 0; i < len; i++)
        {
        }
    }
    else
    {
        len = min_neighbours_length;
        for (int i = 0; i < len; i++)
        {
        }
    }
}

void get_coordinates(int n)
{
    int row = floor(n / DIM) + 1;
    int column = n % DIM;
    if (column == 0)
    {
        row -= 1;
        column = DIM;
    }
    coordinates[0] = row;
    coordinates[1] = column;
}

void get_coordinates_temp(int n)
{
    int row = floor(n / DIM) + 1;
    int column = n % DIM;
    if (column == 0)
    {
        row -= 1;
        column = DIM;
    }
    temp_coordinates[0] = row;
    temp_coordinates[1] = column;
}

int get_number(int r, int c)
{
    int n = r * DIM - (DIM - c);
    return n;
}

int get_md(int n)
{
    get_coordinates(n);
    int row_index = coordinates[0] - 1;
    int column_index = coordinates[1] - 1;
    if (commingBack)
    {
        return rev_md[row_index][column_index];
    }
    else
    {
        return md[row_index][column_index];
    }
}

void change_md(int n, int minimum)
{
    get_coordinates(n);
    int row_index = coordinates[0] - 1;
    int column_index = coordinates[1] - 1;
    if (commingBack)
    {
        rev_md[row_index][column_index] = minimum + 1;
    }
    else
    {
        md[row_index][column_index] = minimum + 1;
    }
}

void get_neighbours(int pos)
{
    int i = 0;

    get_coordinates(pos);
    get_coordinates_temp(pos + 1);
    if (coordinates[0] == temp_coordinates[0])
    { // and not(search(maze, position, position+1))){

        neighbour.cell_number = pos + 1;
        neighbour.dir = 1;
        neighbour.maze_distance = get_md(pos + 1);
        neighbours[i] = neighbour;
        i++;
    }
    get_coordinates_temp(pos - 1);
    if (coordinates[0] == temp_coordinates[0])
    { // and not(search(maze, position, position - 1))){

        neighbour.cell_number = pos - 1;
        neighbour.dir = 2;
        neighbour.maze_distance = get_md(pos - 1);
        neighbours[i] = neighbour;
        i++;
    }
    if (pos + DIM < TOTAL)
    { // and not(search(maze, position, position + DIM))){

        neighbour.cell_number = pos + DIM;
        neighbour.dir = 3;
        neighbour.maze_distance = get_md(pos + DIM);
        neighbours[i] = neighbour;
        i++;
    }
    if (pos - DIM > 0)
    { // and not(search(maze, position, position - DIM))){

        neighbour.cell_number = pos - DIM;
        neighbour.dir = 4;
        neighbour.maze_distance = get_md(pos - DIM);
        neighbours[i] = neighbour;
        i++;
    }
    neighbours_length = i;
}

void initialize_final_maze()
{
    for (int i = 0; i < TOTAL; i++)
    {
        for (int j = 0; j < 4; j++)
            final_maze[i][j] = -1;
    }
}

int num_walls(int arr[])
{
    int len = 4;
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == -1)
            count++;
    }
    return (4 - count);
}

void maze_update(int maze[][4],int base, int wall_with)
{
    int n_walls = num_walls(maze[base]);
    maze[base][n_walls] = wall_with;
}

void get_available_neighbours(int maze[][4], int current)
{
    int k = 0;

    for (int x = 0; x < neighbours_length; x++)
    {
        if (!(search(walls, current, neighbours[x].cell_number)))
        { // check if not walls
            available_neighbours[k] = neighbours[x];
            k++;
        }
        else
        {
            int should_update = 1;
            for (int z = 0; z < 4; z++)
            {
                if (maze[current][z] == neighbours[x].cell_number)
                {
                    should_update = 0;
                    break;
                }
            }

            if (should_update)
            {
                maze_update(maze,neighbours[x].cell_number, current);
                maze_update(maze,current, neighbours[x].cell_number);
            }
        }
    }
    available_neighbours_length = k;
}

void get_available_neighbours_maze(int maze[][4], int current)
{
    int kk = 0;

    for (int xx = 0; xx < neighbours_length; xx++)
    {
        if (!(search(maze, current, neighbours[xx].cell_number)))
        { // check if not walls
            available_neighbours[kk] = neighbours[xx];
            kk++;
        }
    }
    available_neighbours_length = kk;
}

bool search(int arr[][4], int current, int next)
{
    for (int i = 0; i < 4; i++)
    {
        if (next == arr[current][i])
        {
            return true;
        }
    }
    return false;
}

void get_min_md()
{
    int min_maze_distance = TOTAL;

    for (int i = 0; i < available_neighbours_length; i++)
    {
        if (available_neighbours[i].maze_distance < min_maze_distance)
        {
            min_maze_distance = available_neighbours[i].maze_distance;
        }
    }

    int k = 0;
    for (int i = 0; i < available_neighbours_length; i++)
    {
        if (available_neighbours[i].maze_distance == min_maze_distance)
        {
            min_neighbours[k] = available_neighbours[i];

            k++;
        }
    }
    min_neighbours_length = k;
}

void enqueue(int value)
{
    queue[queue_end] = value;
    queue_end++;
    q_size++;
}

int dequeue(void)
{
    int n = queue[queue_start];
    queue_start++;
    q_size--;
    return n;
}

void initialize_queue()
{

    queue_start = 0;
    queue_end = 0;
    q_size = 0;
}

bool queue_empty(void)
{
    return (queue_start - queue_end > 0);
}

void flood_fill(int maze[][4], int current)
{
    // commingBack ^= true;
    initialize_queue();
    enqueue(current);
    while (q_size > 0)
    {
        int front = dequeue();
        get_neighbours(front);

        get_available_neighbours_maze(maze,front);

        get_min_md();

        if (get_md(front) <= min_neighbours[0].maze_distance)
        {
            change_md(front, min_neighbours[0].maze_distance);
            // Serial.print(" Q appended ");
            for (int i = 0; i < available_neighbours_length; i++)
            {
                enqueue(available_neighbours[i].cell_number);
            }
            // Serial.println("");
        }
        else
        {
            continue;
        }
    }
}

int get_relative_direction(int cell1, int cell2)
{
    get_coordinates(cell1);
    int r1 = coordinates[0];
    int c1 = coordinates[1];
    get_coordinates(cell2);
    int r2 = coordinates[0];
    int c2 = coordinates[1];
    if (r1 == r2)
    {
        if (c1 < c2)
            return DIR_EAST;
        else
            return DIR_WEST;
    }
    else
    {
        if (r1 < r2)
            return DIR_SOUTH;
        else
            return DIR_NORTH;
    }
}

int get_turning_direction(int cur_dir, int new_dir)
{
    if ((cur_dir == 4 && new_dir == 2) || (cur_dir == 2 && new_dir == 3) || (cur_dir == 3 && new_dir == 1) || (cur_dir == 1 && new_dir == 4))
    {
        return 6;
    }
    else if ((cur_dir == 4 && new_dir == 1) || (cur_dir == 1 && new_dir == 3) || (cur_dir == 3 && new_dir == 2) || (cur_dir == 2 && new_dir == 4))
    {
        return 7;
    }

    else if ((cur_dir == 4 && new_dir == 3) || (cur_dir == 3 && new_dir == 4) || (cur_dir == 1 && new_dir == 2) || (cur_dir == 2 && new_dir == 1))
    {
        return 8;
    }

    else if (cur_dir == new_dir)
    {
        return -1;
    }
}

void filterPath()
{

    filtered_path_size = 0;
    for (int i = 0; i < index_; ++i)
    {
        bool found = false;
        for (int j = 0; j < filtered_path_size; ++j)
        {
            if (path[i] == filtered_path[j])
            {
                found = true;
                filtered_path_size = j + 1;
                break;
            }
        }

        if (!found)
        {
            filtered_path[filtered_path_size] = path[i];
            filtered_path_size++;
        }
    }
    Serial.println("Done");

    // Output the filtered array to the Serial Monitor
    Serial.println("Filtered Array:");
    for (int i = 0; i < filtered_path_size; i++)
    {
        Serial.print(filtered_path[i]);
        Serial.print(" ");
    }
    Serial.println();

    return;
    // not clear below block

    for (int i = filtered_path_size - 2; i > -1; i--)
    {
        get_neighbours(current_cell);
        for (int yy = 0; yy < neighbours_length; yy++)
        {
            if (filtered_path[i] == neighbours[yy].cell_number)
            {
                next_cell = neighbours[yy].cell_number;
                next_dir = neighbours[yy].dir;
                int turning_direction = get_turning_direction(current_dir, next_dir);
                /////////Add turning functions///////////////////////
                if (turning_direction == 6)
                { /*Serial.println("Turn Left");*/
                }
                else if (turning_direction == 7)
                { /*Serial.println("Turn Right");*/
                }
                else if (turning_direction == 8)
                { /*Serial.println("Turn Back");*/
                }
                else if (turning_direction == -1)
                { /*Serial.println("Don't turn");*/
                }
                /////////////////////////////////////////////////////

                /////////Go one cell forward//////////////////////////
                // Serial.println("Go one cell forward");
                //////////////////////////////////////////////////////
                current_cell = next_cell;
                current_dir = next_dir;
                break;
            }
        }
    }
    Serial.println("Turn Back");
    current_dir = starting_dir;
}

void fast_run()
{
    int dir1 = starting_dir;
    int dir2;
    int dir_turn;
    fast_run_commands_size = 0;

    Serial.println("Shortest path");
    for (int j = 0; j < shortest_path_size; j++)
    {
        Serial.print(shortest_path[j]);
        Serial.print(" ");
    }

    for (int i = 0; i < shortest_path_size - 1; i++)
    {
        dir2 = get_relative_direction(shortest_path[i], shortest_path[i + 1]);
        dir_turn = get_turning_direction(dir1, dir2);
        fast_run_commands[fast_run_commands_size++] = dir_turn;
        if (dir_turn != -1)
            fast_run_commands[fast_run_commands_size++] = -1;
        dir1 = dir2;
    }

    Serial.println("Fast run commands");
    for (int j = 0; j < fast_run_commands_size; j++)
    {
        Serial.print(fast_run_commands[j]);
        Serial.print(" ");
    }

    // compress fast run
    int tmp_size = 0;

    for (int i = 0; i < fast_run_commands_size; i++)
    {
        fast_run_commands[tmp_size] = fast_run_commands[i];
        if (fast_run_commands[i] == -1)
        {

            while ((i + 1) < fast_run_commands_size && fast_run_commands[i + 1] == -1)
            {
                fast_run_commands[tmp_size]--;
                i++;
            }
        }
        tmp_size++;
    }

    fast_run_commands_size = tmp_size;

    Serial.println("Fast run commands");
    for (int i = 0; i < fast_run_commands_size; i++)
    {
        Serial.print(fast_run_commands[i]);
        Serial.print(" ");
        

        if (PUSH_BUTTON1) // SEARCH
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            return;
        }
        else if (PUSH_BUTTON2) // FAST
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON2 = false;
            return;
        }
        else
        {
            // dont remove;
        }
    }
}

void followPath2()
{
    filtered_path_size = 0;
    for (int i = 0; i < index_; ++i)
    {
        bool found = false;
        for (int j = 0; j < filtered_path_size; ++j)
        {
            if (path[i] == filtered_path[j])
            {
                found = true;
                filtered_path_size = j + 1;
                break;
            }
        }

        if (!found)
        {
            filtered_path[filtered_path_size] = path[i];
            filtered_path_size++;
        }
    }
    Serial.println("Done");

    // Output the filtered array to the Serial Monitor
    Serial.println("Filtered Array:");
    for (int i = 0; i < filtered_path_size; i++)
    {
        Serial.print(filtered_path[i]);
        Serial.print(" ");
    }
    Serial.println();

    int commands_length = 0;
    // calculations
    for (int i = filtered_path_size - 2; i > -1; i--)
    {
        get_neighbours(current_cell);
        for (int yy = 0; yy < neighbours_length; yy++)
        {
            if (filtered_path[i] == neighbours[yy].cell_number)
            {
                next_cell = neighbours[yy].cell_number;
                next_dir = neighbours[yy].dir;
                int turning_direction = get_turning_direction(current_dir, next_dir);
                /////////Add turning functions///////////////////////
                if (turning_direction == 6)
                {
                    commands[commands_length] = 6;
                    commands_length++;
                    commands[commands_length] = -2;
                    commands_length++;
                }
                else if (turning_direction == 7)
                {
                    commands[commands_length] = 7;
                    commands_length++;
                    commands[commands_length] = -2;
                    commands_length++;
                }
                else if (turning_direction == 8)
                {
                    commands[commands_length] = 8;
                    commands_length++;
                    commands[commands_length] = -2;
                    commands_length++;
                }
                else if (turning_direction == -1)
                {
                    commands[commands_length] = -2;
                    commands_length++;
                }
                current_cell = next_cell;
                current_dir = next_dir;
                break;
            }
        }
    }
    commands[commands_length] = 8;
    commands_length++;
    current_dir = starting_dir;

    // start navigation
    Serial.println("Comming Back");
    for (int zz = 0; zz < commands_length; zz++)
    {
        if (commands[zz] == 6)
        { /*Serial.println("Turn Left");*/
        }

        else if (commands[zz] == 7)
        { /*Serial.println("Turn Right");*/
        }

        else if (commands[zz] == 8)
        { /*Serial.println("Turn Back");*/
        }

        else if (commands[zz] == -2)
        {
            //   //Find how many cells the robot can go forward
            int number_of_cells = 0;
            while (commands[zz] == -2)
            {
                number_of_cells += 1;
                zz++;
            }
            zz--;
            // Serial.println("Go "+String(number_of_cells) +" cells forward");
        }
    }
}

void backToStart()
{
    commingBack = true;

    memcpy(tmp_maze_backward, tmp_maze_forward, sizeof(tmp_maze_forward)); // CSA
    updateMD(tmp_maze_backward,current_cell, current_dir);
    // current_cell = 22;
    // current_dir = 2;
    index_ = 0;
    path[index_] = current_cell;
    index_++;

   

    while (true)
    {

        if (path[index_ - 1] != current_cell)
        {
            path[index_] = current_cell;
            index_++;
        }

        if (get_md(current_cell) == 0)
        {
            memcpy(final_maze, tmp_maze_backward, sizeof(tmp_maze_backward)); // CSA
            break;
        } // Reached the center destination

        if (PUSH_BUTTON1)
        { // CSA
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            return;
        }

        get_neighbours(current_cell);

        get_available_neighbours(tmp_maze_backward,current_cell);

        get_min_md();

        if (get_md(current_cell) > min_neighbours[0].maze_distance)
        {
            next_cell = min_neighbours[0].cell_number;
            next_dir = min_neighbours[0].dir;

            int turning_direction = get_turning_direction(current_dir, next_dir);

            /////////Add turning functions///////////////////////
            if (turning_direction == 6)
            { /*Serial.println("Turn Left");*/
            }
            else if (turning_direction == 7)
            { /*Serial.println("Turn Right");*/
            }
            else if (turning_direction == 8)
            { /*Serial.println("Turn Back");*/
            }
            else if (turning_direction == -1)
            { /*Serial.println("Don't turn");*/
            }
            /////////////////////////////////////////////////////

            /////////Go one cell forward//////////////////////////
            // Serial.println("Go one cell forward");
            //////////////////////////////////////////////////////
            current_cell = min_neighbours[0].cell_number;
            current_dir = next_dir;

            delay(500); // remove
        }

        else
        {
            flood_fill(tmp_maze_backward,current_cell);
        }

        // print path
    }

    for (int j = 0; j < index_; j++)
    {
        Serial.print(path[j]);
        Serial.print(" ");
    }
    Serial.println();

    filterPath();
    if (filtered_path_size < shortest_path_size)
    {
        shortest_path_size = filtered_path_size;
        Serial.println("New shortest path");
        for (int j = 0; j < filtered_path_size; j++)
        {
            shortest_path[j] = filtered_path[filtered_path_size - j - 1];
            Serial.print(shortest_path[j]);
            Serial.print(" ");
        }
    }

    Serial.println();
}

void startMouse()
{
    for (int it = 0; it < 10; it++)
    {
        //[MOTOR STOP]
        while (PUSH_BUTTON1 == false && PUSH_BUTTON2 == false)
            delay(100);

        if (PUSH_BUTTON1) //SEARCH
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;            
        }
        else if (PUSH_BUTTON2) //FAST
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON2 = false;
            return;
        }
        else
        {
            // dont remove;
        }

        memcpy(tmp_maze_forward, final_maze, sizeof(final_maze)); // CSA

        commingBack = false;
        updateMD(tmp_maze_forward,starting_cell, starting_dir);
        Serial.println("ITERATION " + String(it + 1));
        delay(100);
        current_cell = starting_cell;
        current_dir = starting_dir;
        index_ = 0;
        path[index_] = current_cell;
        index_++;

       

        while (true)
        {

            if (path[index_ - 1] != current_cell)
            {
                path[index_] = current_cell;
                index_++;
            }

            if (get_md(current_cell) == 0)
            {
                memcpy(final_maze, tmp_maze_forward, sizeof(tmp_maze_forward)); // CSA
                break;
            } // Reached the center destination

            if (PUSH_BUTTON1)
            {                 
                //[turn off motors]               
                break;
            }

            get_neighbours(current_cell);

            get_available_neighbours(tmp_maze_forward,current_cell);

            get_min_md();

            if (get_md(current_cell) > min_neighbours[0].maze_distance)
            {
                next_cell = min_neighbours[0].cell_number;
                next_dir = min_neighbours[0].dir;

                int turning_direction = get_turning_direction(current_dir, next_dir);

                /////////Add turning functions///////////////////////
                if (turning_direction == 6)
                { /*Serial.println("Turn Left");*/
                }
                else if (turning_direction == 7)
                { /*Serial.println("Turn Right");*/
                }
                else if (turning_direction == 8)
                { /*Serial.println("Turn Back");*/
                }
                else if (turning_direction == -1)
                { /*Serial.println("Don't turn");*/
                }
                /////////////////////////////////////////////////////

                /////////Go one cell forward//////////////////////////
                // Serial.println("Go one cell forward");
                //////////////////////////////////////////////////////
                current_cell = min_neighbours[0].cell_number;
                current_dir = next_dir;

                delay(500); //remove
            }

            else
            {
                flood_fill(tmp_maze_forward,current_cell);
            }

            // print path
        }

        if(PUSH_BUTTON1){
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            continue;
        }

        for (int j = 0; j < index_; j++)
        {
            Serial.print(path[j]);
            Serial.print(" ");
        }

        Serial.println();
        // update shortest path

        filterPath();
        if (filtered_path_size < shortest_path_size)
        {
            shortest_path_size = filtered_path_size;
            Serial.println("New shortest path");
            for (int j = 0; j < filtered_path_size; j++)
            {
                shortest_path[j] = filtered_path[j];
                Serial.print(shortest_path[j]);
                Serial.print(" ");
            }
        }

        Serial.println();

        backToStart();
        // followPath2();

        // for (int i = 0; i < 6; ++i) {
        //   for (int j = 0; j < 6; ++j) {
        //       Serial.print(md[i][j]);
        //       Serial.print(" ");
        //   }
        //   Serial.println(" ");
        // }
        //       Serial.println(" ");
        // for (int i = 0; i < 6; ++i) {
        //   for (int j = 0; j < 6; ++j) {
        //       Serial.print(rev_md[i][j]);
        //       Serial.print(" ");
        //   }
        //   Serial.println(" ");
        // }
        // Serial.println(" ");
    }
}

void updateMD(int maze[][4], int img_starting_cell, int img_starting_dir)
{
    int img_current_cell = img_starting_cell;
    int img_current_dir = img_starting_dir;
    index_ = 0;
    path[index_] = img_current_cell;
    index_++;
    while (true)
    {

        if (path[index_ - 1] != img_current_cell)
        {
            path[index_] = img_current_cell;
            index_++;
        }

        if (get_md(img_current_cell) == 0)
        {
            break;
        } // Reached the center destination
        get_neighbours(img_current_cell);

        get_available_neighbours_maze(maze,img_current_cell);

        get_min_md();

        if (get_md(img_current_cell) > min_neighbours[0].maze_distance)
        {
            int img_next_cell = min_neighbours[0].cell_number;
            int img_next_dir = min_neighbours[0].dir;

            /////////////////////////////////////////////////////

            /////////Go one cell forward//////////////////////////
            // Serial.println("Go one cell forward");
            //////////////////////////////////////////////////////
            img_current_cell = img_next_cell;
            img_current_dir = img_next_dir;
        }

        else
        {
            flood_fill(maze,img_current_cell);
        }

        // print path
    }

    // for (int j = 0; j < index_; j++){
    // Serial.print(path[j]);
    // Serial.print(" ");
    // }
    // Serial.println();
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    initialize_final_maze(); // initialize empty 2D array for the maze
    fill_md(DESTINATION);
    // show(md);
    // Serial.println(" ");
    // show(rev_md);
    // get_neighbours(29);
    // get_available_neighbours(29);
    // print_neighbours(1);
    // print_neighbours(2);

    // return;
    // CSA
    pinMode(PUSH_BUTTON1_PIN, INPUT);
    attachInterrupt(PUSH_BUTTON1_PIN, push_button1_isr, FALLING);

    pinMode(PUSH_BUTTON2_PIN, INPUT);
    attachInterrupt(PUSH_BUTTON2_PIN, push_button2_isr, FALLING);

    while(PUSH_BUTTON1==false && PUSH_BUTTON2==false)
        delay(100);

    if(PUSH_BUTTON1){
        delay(DEBOUNCE_DELAY);
        PUSH_BUTTON1 = false;
        starting_dir = DIR_EAST;
        Serial.println("Start Dir EAST");
    }else if(PUSH_BUTTON2){
        delay(DEBOUNCE_DELAY);
        PUSH_BUTTON2 = false;
        starting_dir = DIR_SOUTH;
        Serial.println("Start Dir SOUTH");
    }else{
        // dont remove;
    }

    //beep before start

    
}

void loop()
{
    startMouse();
    Serial.println("Search END");
    fast_run();
}

void show(int arr[][DIM])
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (arr[i][j] < 10)
                Serial.print("  ");
            else
                Serial.print(" ");
            Serial.print(arr[i][j]);
        }
        Serial.println();
    }
}