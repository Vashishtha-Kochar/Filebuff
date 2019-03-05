# include "filebuff.h"
# include <string>
# define FILENAME "CM.zip"

int main(int argc, char const *argv[])
{   
    // Declare the object from filebuff class
    fileBuff* fileObject = NULL;
    // fileBuff** output = NULL;
    void *dataptr = NULL;

    // Open the file
    FILE * archiveFilePointer = NULL;
    size_t fileSize = 0;
    archiveFilePointer = fopen (FILENAME,"r");
    
    if (archiveFilePointer==NULL){
        std::cout<<"Error: could not open "<<FILENAME<<std::endl;
        return 0;
    }

    fseek(archiveFilePointer, 0L, SEEK_END); 
  
    // calculating the size of the file 
    fileSize = ftell(archiveFilePointer);
    std::cout<<FILENAME<<" has the size of "<<fileSize<<" bytes"<<std::endl;

    //Sets the position indicator associated with archiveFilePointer to the beginning of the file.
    rewind(archiveFilePointer);

    //Allocate appropriate space to dataptr to store contents of the stream
    dataptr = (char*) malloc (sizeof(char)*fileSize);

    // Start reading
    int noRead = fread(dataptr,1,fileSize,archiveFilePointer);
    std::cout<<"No of items read: "<<noRead<<std::endl;

    // Create object
    fileObject =  new fileBuff(dataptr, fileSize);
    int noOfFiles = fileObject -> list();
    // output = new fileBuff*[noOfFiles] ();
    // std::cout<<"Decompressed: "<<sizeof(output[10]);

    fileBuff output[7];
    fileObject -> decompress(output);
    for(int i = 0; i < 7; i++){
        const char* filename = ("output" + std::to_string(i) + (i<4 ? ".pdf": ".jpeg")).c_str();
        archiveFilePointer = fopen (filename,"wb");
        if (archiveFilePointer==NULL){
            std::cout<<"Error: could not open "<<filename<<std::endl;
            continue;
        }
        int noWritten = fwrite(output[i].getdata(),1,output[i].getdatasize(),archiveFilePointer);
        std::cout<<"No of items written: "<<noWritten<<std::endl;
    }
    // //close file
    free(fileObject);
    fclose (archiveFilePointer);
    free (dataptr);
    return 0;
}
 