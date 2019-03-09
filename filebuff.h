// basic file operations
#include <sys/stat.h>
#include <iostream>
#include <execinfo.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class fileBuff
{
    private:
        void* data;
        size_t data_size;

    public:

        fileBuff();
        fileBuff(void* temp_data, size_t temp_size);
        ~fileBuff();

        void set(void* temp_data, size_t temp_size);
        void* getdata(){return data;}
        size_t getdatasize(){return data_size;}
        
        size_t Read( const char* filePath);
        size_t Write( const char* filePath);
        std::vector<std::string>  list();
        void decompress(std::vector<fileBuff*> &output);
};

// Default Constructor
fileBuff::fileBuff()
{
    data = nullptr;
    data_size = 0; 
}

// Constructor with both arguements
fileBuff::fileBuff(void* temp_data, size_t temp_size)
{
   set(temp_data,temp_size);
}

//Method to set data and datasize
void fileBuff::set(void* temp_data, size_t temp_size){
    data = temp_data;
    data_size = temp_size;
};

// Method to read to Filebuff from a filepath
size_t fileBuff::Read( const char* filePath)
{
    // Open the file
    FILE * archiveFilePointer = NULL;
    size_t fileSize = 0;
    archiveFilePointer = fopen (filePath,"r");
    
    if (archiveFilePointer==NULL){
        std::cout<<"Error: could not open "<<filePath<<std::endl;
        return 0;
    }

    fseek(archiveFilePointer, 0L, SEEK_END); 
  
    // calculating the size of the file 
    fileSize = ftell(archiveFilePointer);
    std::cout<<filePath<<" has the size of "<<fileSize<<" bytes"<<std::endl;

    //Sets the position indicator associated with archiveFilePointer to the beginning of the file.
    rewind(archiveFilePointer);

    //Allocate appropriate space to dataptr to store contents of the stream
    data = (char*) malloc (sizeof(char)*fileSize);

    // Start reading from the file to the fileBuff object
    data_size = fread(data,1,fileSize,archiveFilePointer);
    std::cout<<"No of items read: "<<data_size<<std::endl;

    fclose (archiveFilePointer);
    return data_size;
}

// Method to write to file from a Filebuff
size_t fileBuff::Write( const char* filePath)
{
    // Open the file
    FILE * archiveFilePointer = NULL;
    archiveFilePointer = fopen (filePath,"wb");
    if (archiveFilePointer==NULL)
    {
        std::cout<<"Error: could not write to "<<filePath<<std::endl;
    }

    int noWritten = fwrite(data,1,data_size,archiveFilePointer);
    std::cout<<"No of bytes written: "<<noWritten<<std::endl;

    fclose (archiveFilePointer);
    return noWritten;
}

// List items in an archive
std::vector<std::string> fileBuff::list()
{

    // Obtain an initialized archive object
    struct archive *a = archive_read_new();
    int numberOfFiles = 0;
    std::vector<std::string> listOfItems;

    // Obtain a archive_entry object to read individual entries of the archive
    struct archive_entry *entry = nullptr;

    // For archive stored in memory: Enable the gzip compression and tar format support
    int r = archive_read_support_filter_all(a);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 1: "<<archive_error_string(a)<<std::endl;
    }
    r = archive_read_support_format_zip(a);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 2: "<<archive_error_string(a)<<std::endl;
    }
    // Freeze the settings, open the archive, and prepare for reading entries. Return ARCHIVE_OK on success
    r = archive_read_open_memory(a, data, data_size);
    if (r != ARCHIVE_OK) {
        std::cout<<"Error 3: "<<archive_error_string(a)<<std::endl;
    }

    // Read every entry of the archive and Print its path
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        listOfItems.push_back(archive_entry_pathname(entry));
        numberOfFiles++;
        archive_read_data_skip(a);
    }

    // Free the archive object. Return ARCHIVE_OK on success
    r = archive_read_free(a);
    if (r != ARCHIVE_OK){
        std::cout<<"Error 4: "<<archive_error_string(a)<<std::endl;
    }

    return listOfItems;
}

// Decompress the object to a vector of fileBuffs
void fileBuff::decompress(std::vector<fileBuff*> &output){
    
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

    if (r != ARCHIVE_OK) {
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
        }
        if (size == 0)  break;
        output.push_back(new fileBuff(buff, size));
        
        archive_read_data_skip(archive);
    }
    
    // Release resources
    archive_read_free(archive);
}

// Destructor to handle memory leaks
fileBuff::~fileBuff()
{
    if ( data != nullptr )
        free (data);
    data = nullptr;
    data_size = 0;  
}