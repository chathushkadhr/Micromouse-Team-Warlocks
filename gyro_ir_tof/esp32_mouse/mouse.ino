void fill_md(int up_left_cell)
{

    get_coordinates(up_left_cell);
    int i0 = coordinates[0] - 1;
    int j0 = coordinates[1] - 1;
    
    for (int i = 0; i < DIM; i++)
    {
        int x;
        if(i>i0)
            x = i - i0 - 1;
        else
            x = i0 - i;           
       
        for (int j = 0; j < DIM; j++)
        {
            int y;
            if (j > j0)
                y = j- j0 - 1;
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
        if(i>i0)
            x = i - i0 - 1;
        else
            x = i0 - i;           
       
        for (int j = 0; j < DIM; j++)
        {
            int y;
            if (j > j0)
                y = j- j0 - 1;
            else
                y = j0 - j;

            rev_md[i][j] = x + y;
        }
    }

}

void print_neighbours(int sel){
  int len;
  if (sel == 1) {
    len = neighbours_length;
    for (int i = 0; i < len; i++){
    }
  }
  else if (sel == 2) {
    len = available_neighbours_length;
    for (int i = 0; i < len; i++){
    }
  }
  else {
    len = min_neighbours_length;
    for (int i = 0; i < len; i++){
    }
  }
}

void get_coordinates(int n){
  int row = floor(n/DIM)+1;
  int column = n%DIM;
  if(column==0){
    row -= 1;
    column = DIM;
    }  
  coordinates[0] = row;
  coordinates[1] = column;
}

void get_coordinates_temp(int n){ 
  int row = floor(n/DIM) + 1;
  int column = n % DIM;
  if(column==0){
    row -= 1;
    column = DIM;
    }  
  temp_coordinates[0] = row;
  temp_coordinates[1] = column;
}


int get_number(int r, int c){
  int n = r*DIM-(DIM-c);
  return n;
}

int get_md(int n){
   get_coordinates(n);
   int row_index = coordinates[0] - 1;
   int column_index = coordinates[1] - 1;
   if(commingBack){return rev_md[row_index][column_index];}
   else{return md[row_index][column_index];}
}

void change_md(int n,int minimum){
    get_coordinates(n);
    int row_index=coordinates[0]-1;
    int column_index = coordinates[1]-1;
    if(commingBack){rev_md[row_index][column_index] = minimum+1;}
    else{md[row_index][column_index] = minimum+1;}
}

void get_neighbours(int pos){
    int i = 0;

    get_coordinates(pos);
    get_coordinates_temp(pos+1);
    if (coordinates[0] == temp_coordinates[0]){ // and not(search(maze, position, position+1))){
     
        neighbour.cell_number = pos + 1;
        neighbour.dir = 1;
        neighbour.maze_distance = get_md(pos + 1);
        neighbours[i] = neighbour;
        i++;
    }
    get_coordinates_temp(pos-1);
    if (coordinates[0] == temp_coordinates[0]){ //and not(search(maze, position, position - 1))){
        
        neighbour.cell_number = pos - 1;
        neighbour.dir = 2;
        neighbour.maze_distance = get_md(pos - 1);
        neighbours[i] = neighbour;
        i++;
    }
    if (pos + DIM < TOTAL){ //and not(search(maze, position, position + DIM))){
       
        neighbour.cell_number = pos + DIM;
        neighbour.dir = 3;
        neighbour.maze_distance = get_md(pos + DIM);
        neighbours[i] = neighbour;
        i++;
    }
    if (pos - DIM > 0 ){//and not(search(maze, position, position - DIM))){
        
        neighbour.cell_number = pos - DIM;
        neighbour.dir = 4;
        neighbour.maze_distance = get_md(pos - DIM);
        neighbours[i] = neighbour;
        i++;
    }
    neighbours_length=i;
}

void initialize_maze(){
    for (int i = 0; i < TOTAL; i++){
        for (int j = 0; j < 4; j++)
            maze[i][j] = -1;
    }
}

int num_walls(int arr[]){
    int len = 4;  
    int count = 0;
    for (int i = 0; i < len; i++){
        if (arr[i] == -1) count++;
    }
    return (4-count);
}

void maze_update(int base, int wall_with){
    int n_walls = num_walls(maze[base]);
    maze[base][n_walls ] = wall_with;
}
void get_available_neighbours(int current){
    int k = 0;
//    SerialBT.print(current);
//    SerialBT.print(":    ");
    for (int x = 0; x < neighbours_length; x++){

//          if(get_turning_direction(current_dir,neighbours[x].dir)==8){continue;}
          if (checkWall(current_dir, neighbours[x].dir)){ 
            // check if not walls
            available_neighbours[k] = neighbours[x];
//            SerialBT.print(neighbours[x].cell_number);
//            SerialBT.print(" ");
            k++;
          }
//        if (!(search(walls, current, neighbours[x].cell_number))) { 
//            available_neighbours[k] = neighbours[x];
//            k++;
//        }
        else{
            int should_update=1;
            for(int z=0;z<4;z++){
              if (maze[current][z]==neighbours[x].cell_number){
                  should_update=0;
                  break;
                }
              
              }

            if (should_update){
            maze_update(neighbours[x].cell_number, current);
            maze_update(current, neighbours[x].cell_number);}
        }
    }
//    SerialBT.println(" ");
    available_neighbours_length=k;
}

bool checkWall(int current_direction, int neighbour_direction){
  int neighbour_side = get_turning_direction(current_direction,neighbour_direction); 
  
  double l2 = readTOF(leftTOF2),r2 = readTOF(rightTOF2),f=readTOF(frontTOF);
  //SerialBT.println(String(l2)+" "+String(r2)+" "+String(f));
  
    if(neighbour_side==6){
      if(l2<wall_thresh ){return false;}
      else{return true;}
    }
  
     else if(neighbour_side==7){
      if(r2<wall_thresh){return false;}
      else{return true;}
    }
     else if(neighbour_side==-1){
      if(f<wall_thresh){return false;}
      else{return true;}
    }
     else if(neighbour_side== 8){
      return true;
      
    }  
}
 

  
void get_available_neighbours_maze(int current){
    int kk = 0;
    
    for (int xx = 0; xx < neighbours_length; xx++){
        if (!(search(maze,current, neighbours[xx].cell_number))) { // check if not walls
            available_neighbours[kk] = neighbours[xx];
            kk++;
        }
    }
    available_neighbours_length=kk;
}

bool search(int arr[][4],int current,int next){
    for (int i = 0; i < 4; i++){
        if(next == arr[current][i]){
            return true;
            }
        }
    return false;
}

void get_min_md(){
    int min_maze_distance = TOTAL;

    for (int i = 0; i < available_neighbours_length; i++){
        if (available_neighbours[i].maze_distance < min_maze_distance){
          min_maze_distance = available_neighbours[i].maze_distance;
        }
    }

    int k = 0;
    for (int i = 0; i < available_neighbours_length; i++){
        if (available_neighbours[i].maze_distance == min_maze_distance){
            min_neighbours[k] = available_neighbours[i];
            
            k++;
        }
    }
    min_neighbours_length=k;
}




void enqueue(int value){
  queue[queue_end] = value;
  queue_end++;
  q_size++;
}

int dequeue(void){
  int n = queue[queue_start];
  queue_start++;
  q_size--;
  return n;
}

void initialize_queue(){

  queue_start = 0;
  queue_end = 0;
  q_size=0;
}

bool queue_empty(void){
  return (queue_start - queue_end >0);
}

void flood_fill(int current){
    // commingBack ^= true;
    initialize_queue();
    enqueue(current);
    while (q_size>0){
        int front = dequeue();
        get_neighbours(front);

        get_available_neighbours_maze(front);
        
        get_min_md();

   
        if (get_md(front) <= min_neighbours[0].maze_distance){
            change_md(front, min_neighbours[0].maze_distance);
            // Serial.print(" Q appended ");
            for(int i = 0; i < available_neighbours_length; i++){
                enqueue(available_neighbours[i].cell_number);
            }
            // Serial.println("");
        }
        else {continue;}
    }

}

int get_turning_direction(int cur_dir,int new_dir){
    if ((cur_dir==4 && new_dir==2) || (cur_dir==2 && new_dir==3) || (cur_dir==3 && new_dir==1)|| (cur_dir==1 && new_dir==4)){
         return 6;}      
    else if ((cur_dir==4 && new_dir==1) || (cur_dir==1 && new_dir==3) || (cur_dir==3 && new_dir==2)|| (cur_dir==2 && new_dir==4)){
         return 7;}
   
    else if ((cur_dir==4 && new_dir==3) || (cur_dir==3 && new_dir==4) || (cur_dir==1 && new_dir==2)|| (cur_dir==2 && new_dir==1)){
        return 8;}
        
    else if (cur_dir == new_dir) {
        return -1;}
}

void backToStart(){
  commingBack = true;
  updateMD(current_cell, current_dir);
  index_ = 0;
  path[index_] = current_cell;
  index_++;
  while(true){

      if (path[index_-1]!=current_cell){
        path[index_] = current_cell;
        index_++;
      }

      if (get_md(current_cell) == 0) {break;} // Reached the center destination
      
      get_neighbours(current_cell);
      
      get_available_neighbours(current_cell);
      
      get_min_md();

      if (get_md(current_cell) > min_neighbours[0].maze_distance){
        next_cell =  min_neighbours[0].cell_number;
        next_dir = min_neighbours[0].dir;

        int turning_direction = get_turning_direction(current_dir,next_dir);
        /////////Add turning functions///////////////////////
        if(turning_direction==6){turn90(1);delay(100);SerialBT.println("Turn Left");}
        else if(turning_direction==7){turn90(-1);delay(100);SerialBT.println("Turn Right");}
        else if(turning_direction==8){turn90(1);delay(100);turn90(1);delay(100);SerialBT.println("Turn Back");}
        else if(turning_direction==-1){SerialBT.println("Don't turn");}
        /////////////////////////////////////////////////////
        
        /////////Go one cell forward//////////////////////////
        SerialBT.println("Go one cell forward");
          float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
          prev_left = (l1<wall_thresh)||(l2<wall_thresh);
          prev_right = (r1<wall_thresh)||(r2<wall_thresh);
          
          if(l1<70 && l2<70){
            goForwardspecificDistanceLeft();
          }
          else if(r2<70 && r1<70){
            goForwardspecificDistanceRight();
          }
          else if(l1>150 && l2>150 && r1>70 && r2>70 ){
            goForwardspecificDistanceRight();
          }
          else if(r1>150 && r2>150 && l1>70 && l2>70){
            goForwardspecificDistanceLeft();
          }
          else {
            goCell();
          }
          delay(100);          
        //////////////////////////////////////////////////////
        current_cell = min_neighbours[0].cell_number;
        current_dir = next_dir;
      } 

      else {
        flood_fill(current_cell);
      }
  

  //print path

  }

  int turning_direction = get_turning_direction(current_dir,starting_dir);
  /////////Add turning functions///////////////////////
  if(turning_direction==6){turn90(1);delay(100);SerialBT.println("Turn Left");}
  else if(turning_direction==7){turn90(-1);delay(100);SerialBT.println("Turn Right");}
  else if(turning_direction==8){turn90(1);delay(100);turn90(1);delay(100);SerialBT.println("Turn Back");}
  current_dir = starting_dir;
}

void startMouse(){
  current_cell = starting_cell;
  current_dir = starting_dir;
  for (int it = 0; it < 1; it++){
    mpu.update();
    globle_theta=mpu.getAngleZ();
    commingBack = false;
    updateMD(starting_cell, starting_dir);
//    Serial.println("ITERATION "+String(it+1));
//    delay(100);
    index_ = 0;
    path[index_] = current_cell;
    index_++;
    while(true){
        if (path[index_-1]!=current_cell){
          path[index_] = current_cell;
          index_++;
        }


        if (get_md(current_cell) == 0) {break;} // Reached the center destination
        get_neighbours(current_cell);
        
        get_available_neighbours(current_cell);
        
        get_min_md();

        if (get_md(current_cell) > min_neighbours[0].maze_distance){
          next_cell =  min_neighbours[0].cell_number;
          next_dir = min_neighbours[0].dir;

          int turning_direction = get_turning_direction(current_dir,next_dir);

          /////////Add turning functions///////////////////////
          if(turning_direction==6){
             breakNow();
            turn90(1);
            delay(50);
//            SerialBT.println("Turn Left"); 
            }
          else if(turning_direction==7){
            breakNow();;
            turn90(-1);
            delay(50);
//            SerialBT.println("Turn Right"); 
            }
          else if(turning_direction==8){
            breakNow();turn90(1);
            delay(50);
            turn90(1);
            delay(50);
//            SerialBT.println("Turn Back");
            }
          else if(turning_direction==-1){
           //SerialBT.println("Don't turn"); 
            }
          /////////////////////////////////////////////////////
          
          /////////Go one cell forward//////////////////////////
          SerialBT.println("Go one cell forward");
          goCellDevel();
          
//          float r1 = readTOF(rightTOF1),r2 = readTOF(rightTOF2),l1 =readTOF(leftTOF1), l2 = readTOF(leftTOF2);
//          
//          prev_left = (l1<wall_thresh)||(l2<wall_thresh);
//          prev_right = (r1<wall_thresh)||(r2<wall_thresh);
//          
//          if(l1<70 && l2<70){
//            goForwardspecificDistanceLeft();
//          }
//          else if(r2<70 && r1<70){
//            goForwardspecificDistanceRight();
//          }
//          else if(l1>150 && l2>150 && r1>70 && r2>70 ){
//            goForwardspecificDistanceRight();
//          }
//          else if(r1>150 && r2>150 && l1>70 && l2>70){
//            goForwardspecificDistanceLeft();
//          }
//          else {
//            goCellDevel();
//            goCell();
//          }
//          delay(100);    
          //////////////////////////////////////////////////////
//          blinkLED(1);
          current_cell = min_neighbours[0].cell_number;
          current_dir = next_dir;
        } 

        else {
          flood_fill(current_cell);
        }


    //print path

    }
    
    for (int j = 0; j < index_; j++){
    SerialBT.print(path[j]);
    SerialBT.print(" ");
    }
    SerialBT.println();
    blinkLED(5);
    backToStart();

  }

  
}

void updateMD(int img_starting_cell, int img_starting_dir){
    int img_current_cell = img_starting_cell;
    int img_current_dir = img_starting_dir;
    index_ = 0;
    path[index_] = img_current_cell;
    index_++;
    while(true){

        if (path[index_-1]!=img_current_cell){
          path[index_] = img_current_cell;
          index_++;
        }


        if (get_md(img_current_cell) == 0) {break;} // Reached the center destination
        get_neighbours(img_current_cell);
        
        get_available_neighbours_maze(img_current_cell);
        
        get_min_md();

        if (get_md(img_current_cell) > min_neighbours[0].maze_distance){
          int img_next_cell =  min_neighbours[0].cell_number;
          int img_next_dir = min_neighbours[0].dir;

          /////////////////////////////////////////////////////
          
          /////////Go one cell forward//////////////////////////
          //Serial.println("Go one cell forward");
          //////////////////////////////////////////////////////
          img_current_cell = img_next_cell;
          img_current_dir = img_next_dir;
        } 

        else {
          flood_fill(img_current_cell);
        }


    //print path

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
                 SerialBT.print("  ");
            else
                SerialBT.print(" ");
            SerialBT.print(arr[i][j]);
        }
         SerialBT.println();
    }
}

void definDestination(){
  int count = 0;
  for (int xx=0;xx<10;xx++){
    float f = readTOF(frontTOF);
    if(f<400){
      count++;
    }
    SerialBT.println(f);
    delay(100);
  }

  if(count>5){
    DESTINATION = 54;
  }
  else{
    DESTINATION = 56;
  }
  SerialBT.print("Destination: ");
  SerialBT.println(DESTINATION);
}
