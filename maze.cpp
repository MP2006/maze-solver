/* 
maze.cpp
*/

#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
   int rows, cols, result;
   char** mymaze=NULL;
   
   if(argc < 2)
   {
       cout << "Please provide a maze input file" << endl;
       return 1;
   }
  
   mymaze = read_maze(argv[1], &rows, &cols); // <---TASK: COMPLETE THIS FOR CHECKPOINT 1
   
   if (mymaze == NULL) {
      cout << "Error, input format incorrect." << endl;
      return 1;
   }
	 //================================
   // When working on Checkpoint 4, you will call maze_search here.
   // But for Checkpoint 1, just assume we found the path.
  
   result = maze_search(mymaze, rows, cols); // <--- TASK: CHANGE THIS FOR CHECKPOINT 4
	
	
   // examine value returned by maze_search and print appropriate output
   if (result == 1) { // path found!
      print_maze(mymaze, rows, cols);
   }
   else if (result == 0) { // no path :(
      cout << "No path could be found!" << endl;
   }
   else { // result == -1
      cout << "Invalid maze." << endl;
   }

	 //================================
   // ADD CODE BELOW
   // to delete all memory that read_maze allocated: CHECKPOINT 2
   for(int i = 0; i < rows; i++){
      delete[] mymaze[i];
   }
   delete[] mymaze;
  
   return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 * -1 if invalid maze (not exactly one S and one F)
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 * NOTE: don't forget to deallocate memory in here too!
 *************************************************/
int maze_search(char** maze, int rows, int cols) 
{
  // *** You complete **** CHECKPOINT 4
  int numS = 0;
  int numF = 0;
  Location start;
  Location finish;

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      if(maze[i][j] == 'S'){
        start.row = i;
        start.col = j;
        numS++;
      }
      if(maze[i][j] == 'F'){
        finish.row = i;
        finish.col = j;
        numF++;
      }
    }
  }

  if((numS != 1)||(numF != 1)){
    return -1;
  }
  
  int mazeSize = rows*cols;

  Queue pathQueue = Queue(mazeSize);

  // initialize the predecessor (-1, -1)
  // initialize the explored array. 0 is unexplored, 1 is already explored
  Location** predecessor;
  int** explored;
  
  predecessor = new Location*[rows];
  explored = new int*[rows];

  for(int i = 0; i < rows; i++){
    predecessor[i] = new Location[cols];
    explored[i] = new int[cols];
    for(int j = 0; j < cols; j++){
      predecessor[i][j].row = -1;
      predecessor[i][j].col = -1;
      explored[i][j] = 0;
    }
  }
  
  pathQueue.add_to_back(start);
  Location loc;
  bool endMaze = false;
  int searchRow, searchCol;

  while(!pathQueue.is_empty() && !endMaze){
    loc = pathQueue.remove_from_front();
    for(int direction = 0; direction < 4; direction++){
      if(direction == 0){ // north
        searchRow = loc.row-1;
        searchCol = loc.col;
      }
      if(direction == 1){ // west
        searchRow = loc.row;
        searchCol = loc.col-1;
      }
      if(direction == 2){ // south
        searchRow = loc.row+1;
        searchCol = loc.col;
      }
      if(direction == 3){ // east
        searchRow = loc.row;
        searchCol = loc.col+1;
      }
      if((searchRow >= 0)&&(searchRow < rows)&&(searchCol >= 0)&&(searchCol < cols)){
        if((maze[searchRow][searchCol] == '.')||(maze[searchRow][searchCol] == 'F')){
          if(explored[searchRow][searchCol] == 0){
            explored[searchRow][searchCol] = 1;
            Location curr;
            curr.row = searchRow;
            curr.col = searchCol;
            pathQueue.add_to_back(curr);
            predecessor[searchRow][searchCol] = loc;

            if(maze[searchRow][searchCol] == 'F'){
              endMaze = true;
              break;
            }
          }
        }
      }
    }
  }

  int returnValue;
  loc = finish;

  if(endMaze){
    while(maze[loc.row][loc.col] != 'S'){
      if(maze[loc.row][loc.col] != 'F'){
        maze[loc.row][loc.col] = '*';
      }  
      loc = predecessor[loc.row][loc.col];
    }
    returnValue = 1;
  }
  else{
    returnValue = 0;
  }

  for(int i = 0; i < rows; i++){
    delete[] predecessor[i];
    delete[] explored[i];
  }

  delete[] predecessor;
  delete[] explored;

  return returnValue;

 // DELETE this stub, it's just for Checkpoint 1 to compile.
}
