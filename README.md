# Filebuff
It will be used to decompress a stream of data on the go.
This is a wrapper around libarchive in c++.

# Filebuff class (filebuff.cpp)
Data members:
   * `void* data` : It is a pointer to the memory location of the stream of data
   * `std::size_t data_size` : Number of bytes of data

Constructors:
   * `fileBuff()` : Instantiate the object with `data` being a null pointer and `data_size` being 0
   * `fileBuff(void* data, size_t size)` : Instantiate the object with given `data` and`data_size`

Member functions:
   * `void set(void* data, size_t size)` : Set the `data` and`data_size`
   * `std::size_t Read( std::string filename)` : Reads from the `filename` to the object
   * `std::size_t Write( std::string filename)` : Writes to the `filename` from the object
   
Note : `filename` is a relative path from the directory

   * `std::vector<std::string>  list()` : To list the directories and files of `data`
   * `void compress(std::vector<fileBuff*> &input)` : Compress all the fileBuffs to one
   * `std::vector<fileBuff*> decompress()` : Decompress the fileBuff to a vector of fileBuffs
   * `void* getdata()` : Returns `data`
   * `std::size_t getdatasize()` :  Returns `data_size`

# Use it from the terminal
NAME

&nbsp;&nbsp;&nbsp;&nbsp;Filebuff

SYNOPSIS
   
&nbsp;&nbsp;&nbsp;&nbsp;`./filebuff [-l filename...] [-c filename...] [-d filename...]`

DESCRIPTION

&nbsp;&nbsp;&nbsp;&nbsp;To read, list, compress or decompress a stream of data

OPTIONS

Note : `filename` is a relative path from the directory. It should be a zip archive if you want to decompress or a list of filenames if you want to compress.

&nbsp;&nbsp;&nbsp;&nbsp;`-l filename...` : To list the files and directories in the archive `filename`
&nbsp;&nbsp;&nbsp;&nbsp;`-c filename...` : To compress all the `filename` to `Output/output.zip`
&nbsp;&nbsp;&nbsp;&nbsp;`-d filename...` : To decompress the files specified to `Output` directory preserving the filenames


# Use it as a library
1. First install libarchive and extract it to a new folder "libarchive" inside your directory
2. Copy the files from this repository to your directory
3. `#include "filebuff.h"` : Include the header at the start of your file
4. `fileBuff* fileObject;` : Declare a fileBuff pointer
5. `fileObject->func()` : To call a function from fileBuff class
6. `gcc -o output main.cpp filebuff.cpp inputParser.cpp -larchive -lstdc++` : To compile your main with `filebuff.cpp` and `inputParser.cpp` to `output`
7. Then you can run `output` from terminal and pass appropriate arguments(as per use)
