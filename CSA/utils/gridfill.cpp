
#include <iostream>

using namespace std;

#define LENGTH 8

void fill_grid(int arr[][LENGTH], int i0, int j0)
{
    for (int i = 0; i < LENGTH; i++)
    {
        int x;
        if(i>i0)
            x = i - i0 - 1;
        else
            x = i0 - i;           
       
        for (int j = 0; j < LENGTH; j++)
        {
            int y;
            if (j > j0)
                y = j- j0 - 1;
            else
                y = j0 - j;

            arr[i][j] = x + y;
        }
        cout << endl;
    }
}

void show(int arr[][LENGTH])
{
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            if (arr[i][j] < 10)
                cout << "  ";
            else
                cout << " ";
            cout << arr[i][j];
        }
        cout << endl;
    }
}

int main()
{
    int arr[LENGTH][LENGTH];
   
    // 0x4 cell in the middle
    fill_grid(arr, LENGTH / 2 - 1, LENGTH / 2 - 1);

    // custom 0x4 cell location
    // fill_grid(arr,5, 3);

    show(arr);
    return 0;
}