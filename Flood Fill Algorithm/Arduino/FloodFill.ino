#define dim 6
#define total 37


int walls[total][3] =  {{0,0,0},
{2, 0, 0},
{1, 3, 0},
{2, 0, 0},
{10, 0, 0},
{11, 0, 0},
{0, 0, 0},
{8, 0, 0},
{7, 0, 0},
{15, 0, 0},
{16, 4, 0},
{5, 0,0},
{0, 0,0},
{19, 0,0},
{15,0, 0},
{14, 9,0},
{10, 17, 0},
{16, 18, 23},{17, 0,0},{13, 0, 0},{21, 0, 0},{20, 27, 0},{28, 0, 0},{29, 17, 0},{0, 0, 0},{26,0, 0},{25, 27, 0},{26, 21, 0},{22, 34, 0},{23, 30, 0},{29, 0, 0},{32, 0, 0},{31, 0, 0},{0,0, 0},{28, 35, 0},{34, 0, 0},{0, 0, 0}};

int maze[total][3] ;

typedef struct Cell{
    int cell_number;
    int dir;
    int maze_distance;
} cell;

cell neighbour;

cell neighbours[4] = {{0, 0, total}, {0, 0, total}, {0, 0, total}, {0, 0, total}};
int neighbours_length;

cell available_neighbours[4] = {{0, 0, total}, {0, 0, total}, {0, 0, total}, {0, 0, total}};
int available_neighbours_length;

cell min_neighbours[4] = {{0, 0, total}, {0, 0, total}, {0, 0, total}, {0, 0, total}};
int min_neighbours_length;

int starting_cell = 1;
int current_cell;

int coordinates[2];
int temp_coordinates[2];

int md[6][6] = {{4,3,2,2,3,4},
      {3,2,1,1,2,3,},
      {2,1,0,0,1,2},
      {2,1,0,0,1,2},
      {3,2,1,1,2,3},
      {4,3,2,2,3,4}};

int queue[total];
int queue_start = 0;
int queue_end = 0;
int q_size=0;

void print_neighbours(int sel){
  int len;
  if (sel == 1) {
    len = neighbours_length;
    for (int i = 0; i < len; i++){
      Serial.print(neighbours[i].cell_number);
      Serial.print(" ");
    }
    Serial.println();
  }
  else if (sel == 2) {
    len = available_neighbours_length;
    for (int i = 0; i < len; i++){
      Serial.print(available_neighbours[i].cell_number);
      Serial.print(" ");
    }
    Serial.println();
  }
  else {
    len = min_neighbours_length;
    for (int i = 0; i < len; i++){
      Serial.print(min_neighbours[i].cell_number);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void get_coordinates(int n){
  int row = floor(n/dim)+1;
  int column = n%dim;
  if(column==0){
    row -= 1;
    column = dim;
    }  
  coordinates[0] = row;
  coordinates[1] = column;
}

void get_coordinates_temp(int n){ 
  int row = floor(n/dim) + 1;
  int column = n % dim;
  if(column==0){
    row -= 1;
    column = dim;
    }  
  temp_coordinates[0] = row;
  temp_coordinates[1] = column;
}


int get_number(int r, int c){
  int n = r*dim-(dim-c);
  return n;
}

int get_md(int n){
   get_coordinates(n);
   int row_index = coordinates[0] - 1;
   int column_index = coordinates[1] - 1;
   return md[row_index][column_index];
}

void change_md(int n,int minimum){
    get_coordinates(n);
    int row_index=coordinates[0]-1;
    int column_index = coordinates[1]-1;
    md[row_index][column_index] = minimum+1;
}

void get_neighbours(int pos){
    int i = 0;
    // dir:
    // 1 - east
    // 2 - west
    // 3 - south
    // 4 - north
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
    if (pos + dim < total){ //and not(search(maze, position, position + dim))){
       
        neighbour.cell_number = pos + dim;
        neighbour.dir = 3;
        neighbour.maze_distance = get_md(pos + dim);
        neighbours[i] = neighbour;
        i++;
    }
    if (pos - dim > 0 ){//and not(search(maze, position, position - dim))){
        
        neighbour.cell_number = pos - dim;
        neighbour.dir = 4;
        neighbour.maze_distance = get_md(pos - dim);
        neighbours[i] = neighbour;
        i++;
    }
    neighbours_length=i;
}

void initialize_final_maze(){
    for (int i = 0; i < total; i++){
        for (int j = 0; j < 3; j++)
            maze[i][j] = -1;
    }
}

int num_walls(int arr[]){
    int len = 3;  
    int count = 0;
    for (int i = 0; i < len; i++){
        if (arr[i] == -1) count++;
    }
    return (3-count);
}

void maze_update(int base, int wall_with){
    int n_walls = num_walls(maze[base]);
    maze[base][n_walls ] = wall_with;
}

void get_available_neighbours(int current){
    int k = 0;
    
    for (int x = 0; x < neighbours_length; x++){
        if (!(search(walls, current, neighbours[x].cell_number))) { // check if not walls
            available_neighbours[k] = neighbours[x];
            k++;
        }
        else{
            int should_update=1;
            for(int z=0;z<3;z++){
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
    available_neighbours_length=k;
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

bool search(int arr[][3],int current,int next){
    for (int i = 0; i < 3; i++){
        if(next == arr[current][i]){
            return true;
            }
        }
    return false;
}

void get_min_md(){
    int min_maze_distance = total;

    for (int i = 0; i < available_neighbours_length; i++){
        if (available_neighbours[i].maze_distance < min_maze_distance){
          min_maze_distance = available_neighbours[i].maze_distance;
        }
    }

    int k = 0;
    for (int i = 0; i < available_neighbours_length; i++){
        if (available_neighbours[i].maze_distance == min_maze_distance){
            min_neighbours[k] = available_neighbours[i];
            // Serial.print(available_neighbours[i].cell_number);
            // Serial.print("-");
            // Serial.print(available_neighbours[i].maze_distance);
            // Serial.print(" ");
            
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
    initialize_queue();
    enqueue(current);
    while (q_size>0){
        int front = dequeue();
        // Serial.print("Inside FF: ");
        // Serial.print(front);
        // Serial.print("  MD: ");
        get_neighbours(front);

        get_available_neighbours_maze(front);
//       if(front == 17){
//       for (int i = 0; i < available_neighbours_length; i++){
////      Serial.println(available_neighbours[i].cell_number);
//       }}

        
        get_min_md();
         
//       if(front == 17 || front == 11){
//              Serial.println(" ");
//              Serial.println(front);
//              Serial.println(get_md(front));
//              Serial.println(min_neighbours[0].cell_number);}
        // Serial.print(get_md(front));
        // Serial.print(" MINIMUM: ");
        // Serial.print( min_neighbours[0].maze_distance);
        // Serial.print(" ");
        // Serial.print( min_neighbours[0].cell_number);
   
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



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initialize_final_maze(); // initialize empty 2D array for the maze
  
  // get_neighbours(29);
  // get_available_neighbours(29);
  // print_neighbours(1);
  // print_neighbours(2);

  // return;

  for (int it = 0; it < 10; it++){
    delay(100);
    current_cell = starting_cell;
    
    while(true){
          Serial.print(current_cell);
          Serial.print(" ");
           
          
//        Serial.print(maze[10][0]);
//        Serial.print(" ");
//        Serial.print(maze[10][1]);
//        Serial.print(" ");
//        Serial.print(maze[10][2]);
//        Serial.println(" ");
    
        if (get_md(current_cell) == 0) {break;} // Reached the center destination
        get_neighbours(current_cell);
        
        get_available_neighbours(current_cell);
        
        get_min_md();

 
//        if(current_cell==17){
//        print_neighbours(1);
      //  print_neighbours(2);
//        print_neighbours(3);
//        
//        }
        if (get_md(current_cell) > min_neighbours[0].maze_distance) 
          current_cell = min_neighbours[0].cell_number;
        else {
          flood_fill(current_cell);
        }
    }
    Serial.println(" ");
    // for(int i =0;i<6;i++){
    //   for (int j=0;j<6;j++){
    //     Serial.print(md[i][j]);
    //     Serial.print(" ");
        
    //     }
    //  Serial.println(" "); 
    // }
    //  Serial.println(" "); 
    
      
      }
    for(int i =0;i<6;i++){
    for (int j=0;j<6;j++){
      Serial.print(md[i][j]);
      Serial.print(" ");
      
      }
    Serial.println(" "); 
    }
      
}

void loop() {
  // put your main code here, to run repeatedly:

}
