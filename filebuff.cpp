#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
#include "filebuff.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <iterator>

// Default Constructor
fileBuff::fileBuff()
{
    data = nullptr;
    data_size = 0;
    name = "";
}

// Constructor with both arguements
fileBuff::fileBuff(void* temp_data, size_t temp_size, std::string temp_name)
{
   set(temp_data,temp_size, temp_name);
}

// Destructor to handle memory leaks
fileBuff::~fileBuff()
{
    if ( data != nullptr )
        free (data);
    data = nullptr;
    data_size = 0;
    name = "";
}

//Method to set data and datasize
inline void fileBuff::set(void* temp_data, size_t temp_size, std::string temp_name)
{
    data = temp_data;
    data_size = temp_size;
    name = temp_name;
};

// Method to read to Filebuff from a filepath
size_t fileBuff::Read( std::string filePath)
{
    // Open the file
    FILE * archiveFilePointer = NULL;
    size_t fileSize = 0;
    archiveFilePointer = fopen (filePath.c_str(),"r");
    
    if (archiveFilePointer==NULL){
        return -1;
    }

    fseek(archiveFilePointer, 0L, SEEK_END); 
  
    // calculating the size of the file 
    fileSize = ftell(archiveFilePointer);
    // std::cout<<filePath<<" has the size of "<<fileSize<<" bytes"<<std::endl;

    //Sets the position indicator associated with archiveFilePointer to the beginning of the file.
    rewind(archiveFilePointer);

    //Allocate appropriate space to dataptr to store contents of the stream
    data = (char*) malloc (sizeof(char)*fileSize);

    // Start reading from the file to the fileBuff object
    data_size = fread(data,1,fileSize,archiveFilePointer);
    // std::cout<<"No of items read: "<<data_size<<std::endl;

    name = filePath;

    fclose (archiveFilePointer);
    return data_size;
}

// Method to write to file from a Filebuff
size_t fileBuff::Write()
{
    std::string filePath = "Output/" + name;
    
    // Creating appropriate directories for files if they don't exist
    std::size_t current = filePath.find_last_of('/');
    std::string rootDir = filePath.substr(0, current);
        
    // Check if directory exists already
    struct stat info;
    std::size_t r = stat( rootDir.c_str(), &info );
    if( r == 0)
    {  
        // The directory already exists
        std::cout<<"\n Directory already exists : "<<rootDir;
    }
    else
    {
        // Creating a directory
        if (mkdir(rootDir.c_str(), 0777) == -1)
        {
            std::cerr << "\nError : Could not create directory :"<<rootDir;
            return -1;
        }
        else{
            std::cout << "\nDirectory created : "<<rootDir;
        }
    }

    // Open the file
    FILE * archiveFilePointer = NULL;
    archiveFilePointer = fopen (filePath.c_str(),"wb");
    if (archiveFilePointer==NULL)
    {
        std::cout<<"\nError: could not write to "<<filePath<<std::endl;
    }

    int noWritten = fwrite(data,1,data_size,archiveFilePointer);
    // std::cout<<"No of bytes written: "<<noWritten<<std::endl;

    fclose (archiveFilePointer);
    return noWritten;
}

// List items in an archive
std::vector<std::string> fileBuff::list()
{

    // Obtain an initialized archive object
    struct archive *archive = archive_read_new();
    std::vector<std::string> listOfItems;

    // Obtain a archive_entry object to read individual entries of the archive
    struct archive_entry *entry = nullptr;

    // For archive stored in memory: Enable the gzip compression and tar format support
    int r = archive_read_support_filter_all(archive);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 1: "<<archive_error_string(archive)<<std::endl;
    }
    r = archive_read_support_format_zip(archive);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 2: "<<archive_error_string(archive)<<std::endl;
    }
    // Freeze the settings, open the archive, and prepare for reading entries. Return ARCHIVE_OK on success
    r = archive_read_open_memory(archive, data, data_size);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 3: "<<archive_error_string(archive)<<std::endl;
    }

    // Read every entry of the archive and Print its path
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        // std::cout<<archive_entry_pathname(entry)<<std::endl;
        
        // Check for folders
        // if(archive_entry_size(entry) == 0){
        //     continue;
        // }
        listOfItems.push_back(archive_entry_pathname(entry));
        archive_read_data_skip(archive);
    }

    // Free the archive object. Return ARCHIVE_OK on success
    r = archive_read_free(archive);
    if (r != ARCHIVE_OK){
        std::cout<<"Error 4: "<<archive_error_string(archive)<<std::endl;
    }

    return listOfItems;
}

// Compress a vector of fileBuffs to object
void fileBuff::compress(std::vector<fileBuff*> &input)
{
    // //Obtain an initialized struct archive object
    // struct archive *archive = archive_write_new();
    // struct archive_entry *entry = nullptr;
    
    // //For archive stored in memory: Enable the gzip compression and tar format support
    // int r = archive_write_add_filter_compress(archive);
    // if (r != ARCHIVE_OK) {
    //     std::cout<<"Error : "<<archive_error_string(archive)<<std::endl;
    // }
    // r = archive_write_set_format_zip(archive);
    // if (r != ARCHIVE_OK) {
    //     std::cout<<"Error : "<<archive_error_string(archive)<<std::endl;
    // }

    // // Buffer size = Total Data size
    // size_t buffer_size = 0;
    // for (std::vector<fileBuff*>::iterator ptr = input.begin(); ptr < input.end(); ptr++){
    //     buffer_size += (*ptr)->getdatasize;
    // }

    // data =  malloc(buffer_size);
    // archive_write_open_memory(archive, data, buffer_size , &data_size);

    // for (std::vector<fileBuff*>::iterator ptr = input.begin(); ptr < input.end(); ptr++)
    // {
    //     entry = archive_entry_new();


    //     //Freeze the settings, open the archive, and prepare for reading entries. Return ARCHIVE_OK on success
    //     r = archive_read_open_memory(archive, (*ptr)->getdata(), (*ptr)->getdatasize());

    //     if (r != ARCHIVE_OK) 
    //     {
    //         /* ERROR */
    //         std::cout<<"Error, could not read from the filebuff : "<<archive_error_string(archive);
    //     }
    // }
}

// Decompress the object to a vector of fileBuffs
std::vector<fileBuff*> fileBuff::decompress()
{
    std::vector<fileBuff*> output;
    
    //Obtain an initialized struct archive object
    struct archive *archive = archive_read_new();
    struct archive_entry *entry = nullptr;
    
    //For archive stored in memory: Enable the gzip compression and tar format support
    int r = archive_read_support_filter_all(archive);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 1: "<<archive_error_string(archive)<<std::endl;
    }
    r = archive_read_support_format_zip(archive);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 2: "<<archive_error_string(archive)<<std::endl;
    }
    
    //Freeze the settings, open the archive, and prepare for reading entries. Return ARCHIVE_OK on success
    r = archive_read_open_memory(archive, data, data_size);

    if (r != ARCHIVE_OK) 
    {
        /* ERROR */
        std::cout<<"Error, could not read from the memory location specified : "<<archive_error_string(archive);
    }
    // Read each header of the archive and create a new fileBuff in output for every header 
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        size_t buffsize = archive_entry_size(entry);
        void* buff  = malloc(buffsize);
        size_t size = archive_read_data(archive, buff, buffsize);
        
        if (size < 0) {
            /* ERROR */
            std::cout<<"Error";
        }

        if(size == 0){
            // Directories don't have filebuffs
            continue;
        }
        output.push_back(new fileBuff(buff, size, std::string(archive_entry_pathname(entry))));
        
        archive_read_data_skip(archive);
    }
    
    // Release resources
    archive_read_free(archive);

    return output;
}