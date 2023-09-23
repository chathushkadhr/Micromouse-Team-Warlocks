import tkinter as tk
from tkinter import simpledialog


CELL_SIZE=40
GRID_SIZE = 14
WALL_COUNT=GRID_SIZE+1
WALL_WIDTH=10

walls={}
cells=[]

def wall_click(i,j,o):
    global label
    label.config(text='{},{} {}'.format(i,j,o))
    walls[(i,j,o)].appear= not walls[(i,j,o)].appear
    if(walls[(i,j,o)].appear):
        walls[(i,j,o)].config(bg="black")
    else:
        walls[(i,j,o)].config(bg="white")


def init_ui_root():
    global label
    # Create the main application window
    root = tk.Tk()
    root.title("Button Example")
    root.geometry('{}x{}'.format(CELL_SIZE*(WALL_COUNT+2),CELL_SIZE*WALL_COUNT))

    # Create a label to display the number
    label = tk.Label(root, text="", font=("Helvetica", 24))
    label.place(x=GRID_SIZE*CELL_SIZE+10,y=50)

    button = tk.Button(root,command=draw_maze, text="Maze")
    button.place(x=GRID_SIZE*CELL_SIZE+10,y=100  ,width=100, height=50)

    button = tk.Button(root,command=draw_path, text="Path")
    button.place(x=GRID_SIZE*CELL_SIZE+10,y=200  ,width=100, height=50)

    # button = tk.Button(root,command=clear_path, text="Clear")
    # button.place(x=GRID_SIZE*CELL_SIZE+10,y=300  ,width=100, height=50)

    button = tk.Button(root,command=generate_str, text="Gen")
    button.place(x=GRID_SIZE*CELL_SIZE+10,y=300  ,width=100, height=50)


    for i in range(WALL_COUNT):        
        for j in range(WALL_COUNT):
            if(j!=WALL_COUNT-1 and i!=WALL_COUNT-1):
                cell = tk.Label(root, text=str(i*GRID_SIZE+j+1), font=("Helvetica", 12))
                cell.pack(pady=CELL_SIZE//2, padx=CELL_SIZE//2) 
                cell.place(x=j*CELL_SIZE, y=i*CELL_SIZE, width=CELL_SIZE, height=CELL_SIZE)
                cells.append(cell)

    # Create two black buttons with no background
    for i in range(WALL_COUNT):
        
        for j in range(WALL_COUNT):

            if(j!=WALL_COUNT-1):
                wallH = tk.Button(root,command=lambda t=(i,j,"H"): wall_click(*t), bg="white" ,relief=tk.FLAT)
                wallH.place(x=j*CELL_SIZE, y=i*CELL_SIZE, width=CELL_SIZE, height=WALL_WIDTH)
                wallH.appear=False
                walls[(i,j,"H")]=wallH

            if(i!=WALL_COUNT-1): 
                wallV = tk.Button(root, command=lambda t=(i,j,"V"): wall_click(*t), bg="white",relief=tk.FLAT)
                wallV.place(x=j*CELL_SIZE, y=i*CELL_SIZE, width=WALL_WIDTH, height=CELL_SIZE)
                wallV.appear=False
                walls[(i,j,"V")]=wallV            
            

    return root


def draw_maze():
    # inp_cons = "{{0,0,0,0},{2,0,0,0},{1,3,0,0},{2,0,0,0},{10,0,0,0},{11,0,0,0},{0,0,0,0},{8,0,0,0},{7,0,0,0},{15,0,0,0},{16,4,0,0},{5,0,0,0},{0,0,0,0},{19,0,0,0},{15,0,0,0},{14,9,0,0},{10,17,0,0},{16,18,23,0},{17,0,0,0},{13,0,0,0},{21,0,0,0},{20,27,0,0},{28,0,0,0},{29,17,0,0},{0,0,0,0},{26,0,0,0},{25,27,0,0},{26,21,0,0},{22,34,0,0},{23,30,0,0},{29,0,0,0},{32,0,0,0},{31,0,0,0},{0,0,0,0},{28,35,0,0},{34,0,0,0},{0,0,0,0}}"
    inp_cons=simpledialog.askstring("","Enter Maze")    
    cons = eval(inp_cons.replace('{', '[').replace('}', ']'))
    cons.pop(0)

    for n in range(len(cons)):
        i0 = n // GRID_SIZE
        j0 = n % GRID_SIZE

        for m in range(len(cons[n])):
            p = cons[n][m]-1
            if (p > -1):
                i1 = p // GRID_SIZE
                j1 = p % GRID_SIZE

                if i0==i1: #same row
                    j=(j0+j1+1)//2
                    walls[(i0,j,"V")].config(bg="black")
                    walls[(i0,j,"V")].appear=True
                elif j0==j1: #same col
                    i=(i0+i1+1)//2
                    walls[(i,j0,"H")].config(bg="black")
                    walls[(i,j0,"H")].appear=True

def generate_str():
    global root
    cons=[[0,0,0,0]]
    for i0 in range(GRID_SIZE):
        for j0 in range(GRID_SIZE):
            con=[]

            if i0!=0: #up
                i1=i0-1
                j1=j0

                i=(i0+i1+1)//2

                if walls[(i,j1,"H")].appear:
                    con.append(i1*GRID_SIZE+j1)
            
            if i0!=GRID_SIZE-1: #down
                i1=i0+1
                j1=j0

                i=(i0+i1+1)//2

                if walls[(i,j1,"H")].appear:
                    con.append(i1*GRID_SIZE+j1)   

            if j0!=0: #left
                i1=i0
                j1=j0-1

                j=(j0+j1+1)//2

                if walls[(i1,j,"V")].appear:
                    con.append(i1*GRID_SIZE+j1)

            if j0!=GRID_SIZE-1: #right
                i1=i0
                j1=j0+1
                
                j=(j0+j1+1)//2

                if walls[(i1,j,"V")].appear:
                    con.append(i1*GRID_SIZE+j1)

            for k in range(len(con)):
                con[k]=con[k]+1

            while len(con)<4:
                con.append(0)
            
            cons.append(con)

    out_cons=str(cons)
    out_cons = out_cons.replace('[', '{').replace(']', '}')

    root.clipboard_clear()
    root.clipboard_append(out_cons)
    root.update()



    print(out_cons)

    simpledialog.askstring("",out_cons)

    # print(*cons,sep='\n',end='-------------\n')

def draw_path():
    # inp_path="1,7,13,15,8,9,10,11,11,18,24,23,22"
    inp_path=simpledialog.askstring("","Enter Path")  

    path=map(int,inp_path.split(','))

    clear_path()

    for point in path:
        cells[point-1].config(bg="lime")

def clear_path():
    for cell in cells:
        cell.config(bg="#f0f0f0")

root=init_ui_root() 

# draw_maze()
# draw_path()

root.mainloop()       
        








