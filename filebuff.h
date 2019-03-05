// basic file operations
#include <sys/stat.h>
#include <iostream>
#include <execinfo.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}


class fileBuff{
    private:
        void* data;
        size_t data_size;

    public:

        fileBuff();
        fileBuff(void* temp_data, size_t temp_size);
        ~fileBuff();

        void set(void* temp_data, size_t temp_size);
        size_t Read( const char* filePath);
        void* getdata(){return data;}
        size_t getdatasize(){return data_size;}

        int list();

        fileBuff* decompress(fileBuff* output);
};

// Default Constructor
fileBuff::fileBuff()
{
    data = nullptr;
    data_size = 0; 
}

fileBuff::fileBuff(void* temp_data, size_t temp_size)
{
   set(temp_data,temp_size);
}

//Method to set data and datasize
void fileBuff::set(void* temp_data, size_t temp_size){
    data = temp_data;
    data_size = temp_size;
};

// Method to read from a filepath
size_t fileBuff::Read( const char* filePath)
{
    return 0;
}

// List items in an archive
int fileBuff::list()
{

    // Obtain an initialized archive object
    struct archive *a = archive_read_new();
    int numberOfFiles = 0;

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
        std::cout<<archive_entry_pathname(entry)<<std::endl;
        numberOfFiles++;
        archive_read_data_skip(a);
    }

    // Free the archive object. Return ARCHIVE_OK on success
    r = archive_read_free(a);
    if (r != ARCHIVE_OK){
        std::cout<<"Error 4: "<<archive_error_string(a)<<std::endl;
    }

    return numberOfFiles;
}

fileBuff* fileBuff::decompress(fileBuff* output){
    
    //Obtain an initialized struct archive object
    struct archive *archive = archive_read_new();
    struct archive_entry *entry = nullptr;
    
    //For archive stored in memory: Enable the gzip compression and tar format support
    // For archive stored in memory: Enable the gzip compression and tar format support
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

        return nullptr;
    }
    
    //Get number of files in the compressed file
    // int number_of_files = 0;
    // while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
    //     number_of_files++;
    //     archive_read_data_skip(archive);
    // }

    // r = archive_read_next_header(archive, &entry);
    // if (r != ARCHIVE_OK) {
    //     /* ERROR */
    //     std::cout<<"Error : "<<archive_error_string(archive);;
    // }
    int i = 0;
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        //get the size of the file
        // std::cout<<"Iteration number "<<i+1<<std::endl;
        // std::cout<<archive_entry_pathname(entry)<<std::endl;
        size_t buffsize = archive_entry_size(entry);
        void* buff  = malloc(buffsize);
        size_t size = archive_read_data(archive, buff, buffsize);
        
        output[i].set(buff, size);
        if (size < 0) {
            /* ERROR */
        }
        if (size == 0)
            break;
        archive_read_data_skip(archive);
        i++;
    }
//     for (int i=0;i<1;i++) {
//         //get the size of the file
//         std::cout<<"\n"<<archive_entry_pathname(entry);
//         size_t buffsize = archive_entry_size(entry);
//         void* buff = NULL;
//         size_t size = archive_read_data(archive, buff, buffsize);
        
//         // output[i].set(buff, size);
//         if (size < 0) {
//             /* ERROR */
//         }
//         if (size == 0)
//             break;
        
//         r = archive_read_next_header(archive, &entry);
//         if (r != ARCHIVE_OK) {
//             /* ERROR */
//             break;
//         }
//     }

    archive_read_free(archive);

    return output;
}

fileBuff::~fileBuff()
{
    if ( data != nullptr )
        free (data);
    data = nullptr;
    data_size = 0;  
}