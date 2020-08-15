M-Way B+ Tree
This assignment aims at the implementation of an m-way B+ Tree- a data structure used to
store data as Key- Value pairs and commonly utilized in the implementation of database
indexes. For an m-way B+ tree, all the internal nodes have m pointers. The nodes of B+ trees
store keys in a sorted fashion as well as the pointers to lower level nodes in the tree. The
nodes of an m-way B+ tree are classified into three levels in terms of the ‘root node’, ‘the
intermediate nodes’ and the ‘leaf nodes’. B+ Trees have the unique property of storing data
values only in the leaf nodes and not the intermediate or the root node. The leaf nodes
contain a next sibling pointer for fast iteration through a contiguous block of values. Other
features of the B+ Tree include the fact that root nodes are at least half full, have sibling
pointers that permit sequential search and are balanced. The perpetual challenge during
implementation of an m-way B+ tree is maintaining the ‘degree’ at each node where ‘degree’
can be defined as the maximum number of child nodes that can exist at a point. For instance,
if during insertion, the number of nodes exceeds the degree value, the node is required to
split and rearrange with other nodes within the tree to retain the balanced structural property
of the m-way B+ tree.

## Task
1. Order/Degree Initialization
  - Initialize ‘m’ value i.e. order of the tree dynamically from the input file.
2. Insert Key, Value Pair
  - A call to the Insert() function provided in the input file should dynamically insert the key, value pair in the tree.
3. Delete key
  - a function call provided in the input file to the function responsible for deletion in the source code should facilitate the deletion of the key if found.
4. Search key
  - this is meant to search the value associated with the key provided when the search function is called from the input file.
5. Search key1, key2
  - this is meant to conduct a range search wherein key1 denotes the
lower limit and key2 denotes the upper limit of the range and values that are
associated with keys falling within this range are returned as output.

## Built With
- C++ - The programming language used
- g++ compiler - Compiler Used

## Author
Vriddhi Pai 
