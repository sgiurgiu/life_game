# Simple  Conway's Game of Life implementation

Using C++ and SFML.
The main parts of the logic are as follows
```c++
typedef std::tuple<int,int> point;
typedef std::unordered_map<point,int, hash_tuple::hash<point>> board;
```

The initial state is harcoded in initialize_game() function. The board contains the live points. 
Upon each iteration we calculate which points should be alive next, and we create a new board.
