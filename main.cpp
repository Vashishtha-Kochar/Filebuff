# include "filebuff.h"
# include <string>
# include <vector>
# include <iostream>
# include <fstream>
#include <algorithm>

# define HELP "\nHelp Message"
// std::map< std::string, std::string*> parseCommandLineArguements()
// {
//     // Array of all options available at terminal
//     std::string options[] = {"-h", "--help", "-l", "--list", "-c", "--compress", "-d", "--decompress"};
//     std::map< std::string, std::string*> optionArguements;
//     return optionArguements;
// }

int main(int argc, char **argv)
{
    // Declare the object from filebuff class
    fileBuff* fileObject;
    std::vector<fileBuff*> output;
    std::vector<std::string> listOfFiles;
    std::string filename;
    std::vector<std::string> arguements;
    std::vector<std::string>::iterator list,compress, decompress;

    // Convert char** to vector of string omitting the first element
    for(int i = 1; i < argc; i++ ){
        arguements.push_back(std::string(argv[i]));
    }
    
    // Check if no arguements were passed
    if(arguements.empty())
    {
        std::cout<<"\nToo less arguements"<<HELP;
        return 0;
    }
    
    // Check if asked for help
    if( std::find (arguements.begin(), arguements.end() , "-h") != arguements.end() ||
    std::find (arguements.begin(), arguements.end() , "--help") != arguements.end()){
        std::cout<<HELP;
        std::cout<<"\n";
        return 0;
    };
    
    // Get positions where list, compress or decompress are asked for
    list = std::find (arguements.begin(), arguements.end() , "-l");
    compress = std::find (arguements.begin(), arguements.end() , "-c");
    decompress = std::find (arguements.begin(), arguements.end() , "-d");

    // Create object
    fileObject =  new fileBuff;

    // Check if asked to list files
    if(list != arguements.end()){
        
        // Check if input filenames are given
        if((list+1) == arguements.end() || (list+1) == compress || (list+1) == decompress)
        {
            std::cout<<"\nToo less arguements"<<HELP;
            std::cout<<"\n";
            return 0;
        }
        for(int i = 1; (list+i) != compress && (list+i) != decompress && (list+i) != arguements.end(); i++){
            std::string filename = *(list+i);
            size_t sizeRead = fileObject -> Read(filename);
            if(sizeRead == -1)
            {
                std::cout<<"\nFile Not Found : "<<filename;
                continue;
            }
            listOfFiles = fileObject -> list();
            std::cout<<"\nThe contents of "<<filename<<" are :";
            for(std::vector<std::string>::iterator file = listOfFiles.begin(); file < listOfFiles.end(); file++){
                std::cout<<"\n\t"<<(*file);
            }
        }
    };
    
    // Check if asked to decompress files
    if(decompress != arguements.end()){
        
        // Check if input filenames are given
        if((decompress+1) == arguements.end() || (decompress+1) == compress || (decompress+1) == list)
        {
            std::cout<<"\nToo less arguements for decompress."<<HELP;
            std::cout<<"\n";
            return 0;
        }

        // Decompress each file
        for(int i = 1; (decompress+i) != list && (decompress+i) != compress && (decompress+i) != arguements.end(); i++){
            std::cout<<"\nDecompressing "<<filename;
            std::string filename = *(decompress+i);
            size_t sizeRead = fileObject -> Read(filename);
            if(sizeRead == -1)
            {
                std::cout<<"\nFile Not Found : "<<filename;
                continue;
            }
            listOfFiles = fileObject -> list();
            output = fileObject -> decompress();
            for(int i = 0; i < output.size(); i++){
                if((output[i]->Write("Output/" + listOfFiles[i])) == -1)
                {
                    return 0;
                }
            }
            std::cout<<"\nDecompressed "<<filename;
        }
    };    

    //close file
    delete(fileObject);
    for(int i = 0; i < output.size(); i++) delete(output[i]);

    std::cout<<"\n";
    return 0;
}
 