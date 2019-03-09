# include "filebuff.h"
# include <string>
# define FILENAME "CM.zip"

int main(int argc, char const *argv[])
{   
    // Declare the object from filebuff class
    fileBuff* fileObject = NULL;
    std::vector<fileBuff*> output;
    std::vector<std::string> listOfFiles;

    // Create object
    fileObject =  new fileBuff;
    fileObject -> Read(FILENAME);
    listOfFiles = fileObject -> list();

    // Decompress
    fileObject -> decompress(output);

    // Write to file to demonstrate the decompression
    
    for(int i = 0; i < output.size(); i++){
        const char* filename = listOfFiles[i].c_str();
        output[i]->Write(filename);
    }

    //close file
    free(fileObject);
    
    return 0;
}
 