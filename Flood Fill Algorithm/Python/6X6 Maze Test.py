def get_coordinates(n):
    global dim,total
    row = (n//dim)+1
    column = n%dim
    if column==0:
         row-=1
         column =dim
    return (row,column)

def get_number(r,c):
    n=r*dim-(dim-c)
    return n

def get_md(n):
    return md[get_coordinates(n)[0]-1][get_coordinates(n)[1]-1]

def change_md(n,minimum):
    md[get_coordinates(n)[0]-1][get_coordinates(n)[1]-1]=minimum+1
    
def get_ns(pos):
    num_list=[]
    if get_coordinates(pos)[0]==get_coordinates(pos+1)[0]:
        num_list.append([pos+1,'e',get_md(pos+1)])
    if get_coordinates(pos)[0]==get_coordinates(pos-1)[0]:
        num_list.append([pos-1,'w',get_md(pos-1)])
    if  pos+dim<total:
        num_list.append([pos+dim,'s',get_md(pos+dim)])
    if  pos-dim>0:
        num_list.append([pos-dim,'n',get_md(pos-dim)])
    return num_list
    
def go_straight(direction,pos):
    if direction == 's':
        if pos+dim<total:
            pos=pos+dim
    elif direction == 'n'   :
        if pos-dim>0:
            pos=pos-dim
    elif direction == 'e'   :
        if get_coordinates(pos)[0]==get_coordinates(pos+1)[0]:
            pos=pos+1
    elif direction == 'w'   :     
        if get_coordinates(pos)[0]==get_coordinates(pos-1)[0]:
            pos=pos-1
    return pos

def turn_right(direction):
    if direction == 'n':
        direction = 'e'
    elif direction == 'e':
        direction = 's'
    elif direction == 's':
        direction = 'w'
    elif direction == 'w':
        direction = 'n'    
        
    return direction

def turn_left(direction):
    if direction == 'n':
        direction = 'w'
    elif direction == 'w':
        direction = 's'
    elif direction =='s':
        direction = 'e'
    elif direction =='e':
        direction =='n'    
        
    return direction

def get_turning_direction(current,new):
    if (current=='n' and new=='w') or (current=='w' and new=='s') or (current=='s' and new=='e')or (current=='e' and new=='n'):
        return "left"
    if (current=='n' and new=='e') or (current=='e' and new=='s') or (current=='s' and new=='w')or (current=='w' and new=='n'):
        return "right"
    if (current=='n' and new=='s') or (current=='s' and new=='n') or (current=='e' and new=='w')or (current=='w' and new=='e'):
        return "back"

def get_min_md(ns):
    min_md=float('inf')
    for sq in ns:
        if sq[2]<min_md :
            min_md =sq[2]
    return [min_md,[sq for sq in ns if sq[2] == min_md ]]       

def print_instance(pos):
    dup = maze[get_coordinates(pos)[0]-1][get_coordinates(pos)[1]-1]
    maze[get_coordinates(pos)[0]-1][get_coordinates(pos)[1]-1]="X"
    print ( "\n".join(list(map(str,maze)))+"\n")
    maze[get_coordinates(pos)[0]-1][get_coordinates(pos)[1]-1]=dup

def flood_fill(current):
    known=1
    q=[]
    q.append(current)
    while len(q)!=0 :        
        front=q.pop(0)
        ns = get_ns(front)
        available_ns=[]
        for num in ns:
            if num[0] not in walls[front]:
                available_ns.append(num)
        if known:
            min_md_data = get_min_md(available_ns)
            known=0
        else:
            min_md_data = get_min_md(ns)
        if get_md(front)<=min_md_data[0] :
            change_md(front,min_md_data[0])
            for i in available_ns:
                q.append(i[0])
        else:
            continue     
dim = 6
total=37
    

maze= [[1,2,3,4,5,6],
       [7,8,9,10,11,12],
       [13,14,15,16,17,18],
       [19,20,21,22,23,24],
       [25,26,27,28,29,30],
       [31,32,33,34,35,36]]

md = [[4,3,2,2,3,4],
      [3,2,1,1,2,3,],
      [2,1,0,0,1,2],
      [2,1,0,0,1,2],
      [3,2,1,1,2,3],
      [4,3,2,2,3,4]]

walls={1:[2],
       2:[1,3],
       3:[2],
       4:[10],
       5:[11],
       6:[],
       7:[8],
       8:[7],
       9:[15],
       10:[16,4],
       11:[5],
       12:[],
       13:[19],
       14:[15],
       15:[14,9],
       16:[10,17],
       17:[16,18,23],
       18:[17],
       19:[13],
       20:[21],
       21:[20,27],
       22:[28],
       23:[29,17],
       24:[],
       25:[26],
       26:[25,27],
       27:[26,21],
       28:[22,34],
       29:[23,30],
       30:[29],
       31:[32],
       32:[31],
       33:[],
       34:[28,35],
       35:[34],
       36:[]
    }

discovered_walls={1:[2],
       2:[],
       3:[],
       4:[],
       5:[],
       6:[],
       7:[],
       8:[],
       9:[],
       10:[],
       11:[],
       12:[],
       13:[],
       14:[],
       15:[],
       16:[],
       17:[],
       18:[],
       19:[],
       20:[],
       21:[],
       22:[],
       23:[],
       24:[],
       25:[],
       26:[],
       27:[],
       28:[],
       29:[],
       30:[],
       31:[],
       32:[],
       33:[],
       34:[],
       35:[],
       36:[]
       
    }

#print ( "\n".join(list(map(str,md)))+"\n")
#print_instance(front)
#print ( "\n".join(list(map(str,md)))+"\n")

start=1
#Starts Simulation
current=start
direction="s"
reached=0
for i in range(10):
    current=start
    while(True):
        #print ( "\n".join(list(map(str,md))))   
        print(current)
        if get_md(current)==0:
            break 
        neighbor_squares= get_ns(current)
        available_neighbor_squares=[]
        for num in neighbor_squares:
            if num[0] not in walls[current]:
                available_neighbor_squares.append(num)
            else:
                discovered_walls[num[0]].append(current)
                discovered_walls[current].append(num[0])
                
        minimum_md = get_min_md(available_neighbor_squares)

        if get_md(current)>minimum_md[0]:
            current = minimum_md[1][0][0]
            
        else:
            #print ( "\n".join(list(map(str,md)))+"\n")
            flood_fill(current)
            #print ( "\n".join(list(map(str,md)))+"\n")
        
        print("\n")

            
  
    
print ( "\n".join(list(map(str,md)))+"\n")  











