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

            final_md[i][j] = x + y;
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

            final_rev_md[i][j] = x + y;
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

void maze_update(int maze[][4], int base, int wall_with)
{
    int n_walls = num_walls(maze[base]);
    maze[base][n_walls] = wall_with;
}

void get_available_neighbours(int maze[][4], int current)
{
    int k = 0;

    for (int x = 0; x < neighbours_length; x++)
    {
        if (checkWall(current_dir, neighbours[x].dir))
        {
            // check if not walls
            available_neighbours[k] = neighbours[x];
            //            SerialBT.print(neighbours[x].cell_number);
            //            SerialBT.print(" ");
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
                maze_update(maze, neighbours[x].cell_number, current);
                maze_update(maze, current, neighbours[x].cell_number);
            }
        }
    }
    available_neighbours_length = k;
}

bool checkWall(int current_direction, int neighbour_direction)
{
    int neighbour_side = get_turning_direction(current_direction, neighbour_direction);

    float l = readIR2(leftIR);
    float r = readIR2(rightIR);
    float f = readTOF(frontTOF);
    delay(5);

    if (neighbour_side == 6)
    {
        if (l < IR_THRESH)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    else if (neighbour_side == 7)
    {
        if (r < IR_THRESH)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (neighbour_side == -1)
    {
        if (f < wall_thresh)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (neighbour_side == 8)
    {
        return true;
    }
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

        get_available_neighbours_maze(maze, front);

        get_min_md();

        if (get_md(front) <= min_neighbours[0].maze_distance)
        {
            change_md(front, min_neighbours[0].maze_distance);
            // Serial.print(" Q appended ");
            for (int i = 0; i < available_neighbours_length; i++)
            {
                if (queue_end > (MAX_QUEUE_INDEX - 10))
                {
                    while (PUSH_BUTTON1 == false)
                    {
                        blinkLED(1);
                    }
                    return;
                }
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

    // Serial.println("Shortest path");
    // for (int j = 0; j < shortest_path_size; j++)
    // {
    //     Serial.print(shortest_path[j]);
    //     Serial.print(" ");
    // }

    for (int i = 0; i < shortest_path_size - 1; i++)
    {
        dir2 = get_relative_direction(shortest_path[i], shortest_path[i + 1]);
        dir_turn = get_turning_direction(dir1, dir2);
        fast_run_commands[fast_run_commands_size++] = dir_turn;
        if (dir_turn != -1)
            fast_run_commands[fast_run_commands_size++] = -1;
        dir1 = dir2;
    }

    // Serial.println("Fast run commands");
    // for (int j = 0; j < fast_run_commands_size; j++)
    // {
    //     Serial.print(fast_run_commands[j]);
    //     Serial.print(" ");
    // }

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
    Buzz(startTone);

    // Serial.println("Fast run commands");
    for (int i = 0; i < fast_run_commands_size; i++)
    {
        // Serial.print(fast_run_commands[i]);
        // Serial.print(" ");
        int f_cmd = fast_run_commands[i];
        if (f_cmd < 0)
        {
            f_cmd = -f_cmd;
            for (int i = 0; i < f_cmd; i++)
            {
                if (i == f_cmd - 1)
                {
                    goCellFast(true);
                }
                else
                {
                    goCellFast(false);
                }
            }
        }
        else if (f_cmd == DIR_LEFT)
        {
            turn_align(1);
        }
        else if (f_cmd == DIR_RIGHT)
        {
            turn_align(-1);
        }
        else
        {
            // dont remove;
        }

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
    Buzz(3);
}

void backToStart()
{
    commingBack = true;

    memcpy(tmp_maze_backward, tmp_maze_forward, sizeof(tmp_maze_forward)); // CSA
    memcpy(rev_md, final_rev_md, sizeof(final_md));                        // CSA

    updateMD(tmp_maze_backward, current_cell, current_dir);
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
            memcpy(final_rev_md, rev_md, sizeof(final_md));                   // CSA

            break;
        } // Reached the center destination

        if (PUSH_BUTTON1)
        { // CSA
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            blinkLED(1);
            return;
        }

        get_neighbours(current_cell);

        get_available_neighbours(tmp_maze_backward, current_cell);

        get_min_md();

        if (get_md(current_cell) > min_neighbours[0].maze_distance)
        {
            next_cell = min_neighbours[0].cell_number;
            next_dir = min_neighbours[0].dir;

            int turning_direction = get_turning_direction(current_dir, next_dir);

            /////////Add turning functions///////////////////////
            if (turning_direction == 6)
            {
                turn_align(1);
                //            SerialBT.println("Turn Left");
            }
            else if (turning_direction == 7)
            {
                // breakNow();
                turn_align(-1);
                //            SerialBT.println("Turn Right");
            }
            else if (turning_direction == 8)
            {
                turn_align(1);
                turn_align(1);
                //            SerialBT.println("Turn Back");
            }
            else if (turning_direction == -1)
            {
                // SerialBT.println("Don't turn");
            }
            /////////////////////////////////////////////////////

            /////////Go one cell forward//////////////////////////
            //          SerialBT.println("Go one cell forward");
            //          goCellDevelIR();
            goCellDevelIR();
            correct_direction();

            //////////////////////////////////////////////////////
            current_cell = min_neighbours[0].cell_number;
            current_dir = next_dir;
        }

        else
        {
            flood_fill(tmp_maze_backward, current_cell);
        }

        // print path
    }

    filterPath();
    if (filtered_path_size < shortest_path_size)
    {
        shortest_path_size = filtered_path_size;
        SerialBT.println("New shortest path");
        for (int j = 0; j < filtered_path_size; j++)
        {
            shortest_path[j] = filtered_path[filtered_path_size - j - 1];
            SerialBT.print(shortest_path[j]);
            SerialBT.print(" ");
        }
    }

    SerialBT.println();

    int turning_direction = get_turning_direction(current_dir, starting_dir);
    /////////Add turning functions///////////////////////
    if (turning_direction == 6)
    {
        turn_align(1);
        delay(20);
        //    SerialBT.println("Turn Left");
    }
    else if (turning_direction == 7)
    {
        turn_align(-1);
        delay(20);
        //    SerialBT.println("Turn Right");
    }
    else if (turning_direction == 8)
    {
        turn_align(1);
        //    delay(100);
        turn_align(1);
        //    delay(100);
        //    SerialBT.println("Turn Back");
    }
    current_dir = starting_dir;
}

void startMouse()
{
    for (int it = 0; it < 50; it++)
    {
        stop_motors();

        while (PUSH_BUTTON1 == false && PUSH_BUTTON2 == false)
            delay(100);

        if (PUSH_BUTTON1) // SEARCH
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            blinkLED(1);
            while (PUSH_BUTTON1 == false && PUSH_BUTTON2 == false)
                delay(100);

            if (PUSH_BUTTON1) // Continue
            {
                delay(DEBOUNCE_DELAY);
                PUSH_BUTTON1 = false;
            }
            else if (PUSH_BUTTON2) // Restore Backup
            {
                delay(DEBOUNCE_DELAY);
                PUSH_BUTTON2 = false;

                // restore

                memcpy(final_maze, backup_final_maze, sizeof(backup_final_maze));          // CSA
                memcpy(final_md, backup_final_md, sizeof(backup_final_md));                // CSA
                memcpy(final_rev_md, backup_final_rev_md, sizeof(backup_final_rev_md));    // CSA
                memcpy(shortest_path, backup_shortest_path, sizeof(backup_shortest_path)); // CSA
                shortest_path_size = backup_shortest_path_size;
                longBeep(2);
            }
            else
            {
                // dont remove;
            }
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
        Buzz(startTone);
        memcpy(tmp_maze_forward, final_maze, sizeof(final_maze)); // CSA
        memcpy(md, final_md, sizeof(final_md));

        // Backup
        memcpy( backup_final_maze,final_maze, sizeof(backup_final_maze));          // CSA
        memcpy( backup_final_md,final_md, sizeof(backup_final_md));                // CSA
        memcpy( backup_final_rev_md,final_rev_md, sizeof(backup_final_rev_md));    // CSA
        memcpy( backup_shortest_path,shortest_path, sizeof(backup_shortest_path)); // CSA
        backup_shortest_path_size=shortest_path_size;

        commingBack = false;
        updateMD(tmp_maze_forward, starting_cell, starting_dir);
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
                memcpy(final_md, md, sizeof(final_md));                         // CSA
                Buzz(3);
                break;
            } // Reached the center destination

            if (PUSH_BUTTON1)
            {
                //[turn off motors]
                break;
            }

            get_neighbours(current_cell);

            get_available_neighbours(tmp_maze_forward, current_cell);

            get_min_md();

            if (get_md(current_cell) > min_neighbours[0].maze_distance)
            {
                next_cell = min_neighbours[0].cell_number;
                next_dir = min_neighbours[0].dir;

                int turning_direction = get_turning_direction(current_dir, next_dir);

                /////////Add turning functions///////////////////////
                if (turning_direction == 6)
                {
                    turn_align(1);
                    //            SerialBT.println("Turn Left");
                }
                else if (turning_direction == 7)
                {
                    // breakNow();;
                    turn_align(-1);
                    //            SerialBT.println("Turn Right");
                }
                else if (turning_direction == 8)
                {
                    turn_align(1);
                    turn_align(1);
                    //            SerialBT.println("Turn Back");
                }
                else if (turning_direction == -1)
                {
                    // SerialBT.println("Don't turn");
                }
                /////////////////////////////////////////////////////

                /////////Go one cell forward//////////////////////////
                //          SerialBT.println("Go one cell forward");
                //          goCellDevelIR();
                goCellDevelIR();
                correct_direction();

                //////////////////////////////////////////////////////
                current_cell = min_neighbours[0].cell_number;
                current_dir = next_dir;
            }

            else
            {
                flood_fill(tmp_maze_forward, current_cell);
            }

            // print path
        }

        if (PUSH_BUTTON1)
        {
            delay(DEBOUNCE_DELAY);
            PUSH_BUTTON1 = false;
            blinkLED(1);
            continue;
        }

        //        for (int j = 0; j < index_; j++)
        //        {
        //            Serial.print(path[j]);
        //            Serial.print(" ");
        //        }

        //        Serial.println();
        // update shortest path

        filterPath();
        if (filtered_path_size < shortest_path_size)
        {
            shortest_path_size = filtered_path_size;
            SerialBT.println("New shortest path");
            for (int j = 0; j < filtered_path_size; j++)
            {
                shortest_path[j] = filtered_path[j];
                SerialBT.print(shortest_path[j]);
                SerialBT.print(" ");
            }
        }

        SerialBT.println();

        backToStart();
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

        get_available_neighbours_maze(maze, img_current_cell);

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
            flood_fill(maze, img_current_cell);
        }

        // print path
    }

    // for (int j = 0; j < index_; j++){
    // Serial.print(path[j]);
    // Serial.print(" ");
    // }
    // Serial.println();
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
