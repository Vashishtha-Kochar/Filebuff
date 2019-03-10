# include "filebuff.h"
# include "inputParser.h"
# include <string>
# include <vector>
# include <iostream>
#include <fstream>

inline bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char **argv)
{
    // Declare the object from filebuff class
    fileBuff* fileObject = NULL;
    std::vector<fileBuff*> output;
    std::vector<std::string> listOfFiles;
    std::string filename;

    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h")){
        // Give Help
        std::cout<<"Help message\n";
    }
    filename = input.getCmdOption("-f");
    if (filename.empty()){
        // Ask the user for the archive file to read from
        std::cout<<"Enter the filename to read from:\n";
        std::cin>>filename;
    }

    while(!file_exists(filename)){
        std::cout<<"File Not Found. Enter valid filename to read from:\n";
        std::cin>>filename;
    }
    
    
    // Create object
    fileObject =  new fileBuff;
    fileObject -> Read(filename);
    listOfFiles = fileObject -> list();

    // Decompress
    fileObject -> decompress(output);

    // Write to file to demonstrate the decompression
    
    for(int i = 0; i < output.size(); i++){
        output[i]->Write("Output/" + listOfFiles[i]);
    }

    //close file
    delete(fileObject);
    
    return 0;
}
 