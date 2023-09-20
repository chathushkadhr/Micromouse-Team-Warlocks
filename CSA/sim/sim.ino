#define DIM 10
#define TOTAL DIM *DIM + 1
#define CENTER DIM *(DIM - 1) / 2

#define DESTINATION 54 // Up left corner cell of the destination

#define PUSH_BUTTON_PIN 13
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

bool commingBack = false;

// int walls[TOTAL][4] =  {{0, 0, 0, 0},{2, 0, 0, 0},{1, 3, 0, 0},{2, 0, 0, 0},{10, 0, 0, 0},{11, 0, 0, 0},{0, 0, 0, 0},{8, 0, 0, 0},{7, 0, 0, 0},{15, 0, 0, 0},{16, 4, 0, 0},{5, 0, 0, 0},{0, 0, 0, 0},{19, 0, 0, 0},{15, 0, 0, 0},{14, 9, 0, 0},{10, 17, 0, 0},{16, 18, 23, 0},{17, 0, 0, 0},{13, 0, 0, 0},{21, 0, 0, 0},{20, 27, 0, 0},{28, 0, 0, 0},{29, 17, 0, 0},{0, 0, 0, 0},{26,0, 0, 0},{25, 27, 0, 0},{26, 21, 0, 0},{22, 34, 0, 0},{23, 30, 0, 0},{29, 0, 0, 0},{32, 0, 0, 0},{31, 0, 0, 0},{0,0, 0, 0},{28, 35, 0, 0},{34, 0, 0, 0},{0, 0, 0, 0}};
// int walls[TOTAL][4] = {{0, 0, 0, 0}, {2, 0, 0, 0}, {1, 3, 0, 0}, {2, 0, 0, 0}, {5, 0, 0, 0}, {4, 6, 0, 0}, {5, 0, 0, 0}, {8, 0, 0, 0}, {7, 0, 0, 0}, {15, 10, 0, 0}, {16, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {19, 0, 0, 0}, {15, 0, 0, 0}, {9, 21, 14, 16}, {10, 22, 15, 0}, {18, 0, 0, 0}, {24, 17, 0, 0}, {13, 0, 0, 0}, {26, 0, 0, 0}, {15, 22, 0, 0}, {16, 21, 0, 0}, {24, 0, 0, 0}, {18, 23, 0, 0}, {26, 0, 0, 0}, {20, 25, 27, 0}, {26, 28, 0, 0}, {34, 27, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {28, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
int walls[TOTAL][4] =
    {{0, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {4, 0, 0, 0}, {3, 0, 0, 0}, {15, 0, 0, 0}, {7, 0, 0, 0}, {6, 0, 0, 0}, {0, 0, 0, 0}, {19, 0, 0, 0}, {20, 0, 0, 0}, {21, 0, 0, 0}, {13, 0, 0, 0}, {12, 0, 0, 0}, {24, 0, 0, 0}, {5, 25, 0, 0}, {26, 0, 0, 0}, {27, 18, 0, 0}, {17, 0, 0, 0}, {9, 29, 0, 0}, {10, 0, 0, 0}, {11, 0, 0, 0}, {32, 23, 0, 0}, {33, 22, 24, 0}, {14, 23, 25, 0}, {15, 24, 26, 0}, {16, 36, 25, 0}, {17, 0, 0, 0}, {38, 29, 0, 0}, {19, 28, 30, 0}, {29, 0, 0, 0}, {32, 0, 0, 0}, {22, 31, 0, 0}, {23, 0, 0, 0}, {0, 0, 0, 0}, {36, 0, 0, 0}, {26, 35, 0, 0}, {47, 38, 0, 0}, {28, 48, 37, 0}, {0, 0, 0, 0}, {50, 0, 0, 0}, {42, 0, 0, 0}, {52, 41, 43, 0}, {42, 44, 0, 0}, {54, 43, 45, 0}, {55, 44, 0, 0}, {56, 47, 0, 0}, {37, 46, 48, 0}, {38, 47, 0, 0}, {0, 0, 0, 0}, {40, 60, 0, 0}, {0, 0, 0, 0}, {42, 53, 0, 0}, {52, 54, 0, 0}, {44, 53, 0, 0}, {45, 56, 0, 0}, {46, 66, 55, 0}, {0, 0, 0, 0}, {68, 59, 0, 0}, {69, 58, 0, 0}, {50, 0, 0, 0}, {62, 0, 0, 0}, {72, 61, 63, 0}, {62, 64, 0, 0}, {63, 0, 0, 0}, {75, 66, 0, 0}, {56, 65, 0, 0}, {77, 0, 0, 0}, {58, 0, 0, 0}, {59, 79, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {62, 73, 0, 0}, {72, 74, 0, 0}, {73, 75, 0, 0}, {65, 85, 74, 0}, {0, 0, 0, 0}, {67, 87, 78, 0}, {77, 79, 0, 0}, {69, 89, 78, 0}, {0, 0, 0, 0}, {82, 0, 0, 0}, {92, 81, 83, 0}, {93, 82, 0, 0}, {94, 0, 0, 0}, {75, 95, 0, 0}, {87, 0, 0, 0}, {77, 86, 88, 0}, {87, 0, 0, 0}, {79, 99, 90, 0}, {89, 0, 0, 0}, {0, 0, 0, 0}, {82, 0, 0, 0}, {83, 0, 0, 0}, {84, 0, 0, 0}, {85, 96, 0, 0}, {95, 97, 0, 0}, {96, 0, 0, 0}, {99, 0, 0, 0}, {89, 98, 0, 0}, {0, 0, 0, 0}};
int maze[TOTAL][4];
int final_maze[TOTAL][4]; // CSA
bool PUSH_BUTTON = false; // CSA

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
int shortest_path_size = TOTAL;
int filtered_path_size = 0;

void push_button_isr()
{
    PUSH_BUTTON = true;
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

void maze_update(int base, int wall_with)
{
    int n_walls = num_walls(maze[base]);
    maze[base][n_walls] = wall_with;
}

void get_available_neighbours(int current)
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
                maze_update(neighbours[x].cell_number, current);
                maze_update(current, neighbours[x].cell_number);
            }
        }
    }
    available_neighbours_length = k;
}

void get_available_neighbours_maze(int current)
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

void flood_fill(int current)
{
    // commingBack ^= true;
    initialize_queue();
    enqueue(current);
    while (q_size > 0)
    {
        int front = dequeue();
        get_neighbours(front);

        get_available_neighbours_maze(front);

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
    updateMD(current_cell, current_dir);
    // current_cell = 22;
    // current_dir = 2;
    index_ = 0;
    path[index_] = current_cell;
    index_++;

    memcpy(maze, final_maze, sizeof(maze)); // CSA

    while (true)
    {

        if (path[index_ - 1] != current_cell)
        {
            path[index_] = current_cell;
            index_++;
        }

        if (get_md(current_cell) == 0)
        {
            memcpy(final_maze, maze, sizeof(maze)); // CSA
            break;
        } // Reached the center destination

        if (PUSH_BUTTON)
        { // CSA
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON = false;
            //[turn off motors]
            while (PUSH_BUTTON == false)
                ;
            delay(DEBOUNCE_DELAY);
            break;
        }

        get_neighbours(current_cell);

        get_available_neighbours(current_cell);

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
        }

        else
        {
            flood_fill(current_cell);
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
            shortest_path[j] = filtered_path[filtered_path_size-j-1];
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
        commingBack = false;
        updateMD(starting_cell, starting_dir);
        Serial.println("ITERATION " + String(it + 1));
        delay(100);
        current_cell = starting_cell;
        current_dir = starting_dir;
        index_ = 0;
        path[index_] = current_cell;
        index_++;

        memcpy(maze, final_maze, sizeof(maze)); // CSA

        while (true)
        {

            if (path[index_ - 1] != current_cell)
            {
                path[index_] = current_cell;
                index_++;
            }

            if (get_md(current_cell) == 0)
            {
                memcpy(final_maze, maze, sizeof(maze)); // CSA
                break;
            } // Reached the center destination

            if (PUSH_BUTTON)
            { // CSA
                delay(DEBOUNCE_DELAY);
                PUSH_BUTTON = false;
                //[turn off motors]
                while (PUSH_BUTTON == false)
                    ;
                delay(DEBOUNCE_DELAY);
                break;
            }

            get_neighbours(current_cell);

            get_available_neighbours(current_cell);

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
            }

            else
            {
                flood_fill(current_cell);
            }

            // print path
        }

        for (int j = 0; j < index_; j++)
        {
            Serial.print(path[j]);
            Serial.print(" ");
        }

        Serial.println();
        // update shortest path

        filterPath();
        if(filtered_path_size<shortest_path_size){
            shortest_path_size = filtered_path_size;
            Serial.println("New shortest path");
            for (int j = 0; j < filtered_path_size;j++){
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


void updateMD(int img_starting_cell, int img_starting_dir)
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

        get_available_neighbours_maze(img_current_cell);

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
            flood_fill(img_current_cell);
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
    show(md);
    Serial.println(" ");
    show(rev_md);
    // get_neighbours(29);
    // get_available_neighbours(29);
    // print_neighbours(1);
    // print_neighbours(2);

    // return;
    // CSA
    pinMode(PUSH_BUTTON_PIN, INPUT);
    attachInterrupt(PUSH_BUTTON_PIN, push_button_isr, FALLING);

    startMouse();
}

void loop()
{
    // put your main code here, to run repeatedly:
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