// include guard
#ifndef __FILEBUFF_H_INCLUDED__
#define __FILEBUFF_H_INCLUDED__
#include <string>
#include <vector>

class fileBuff
{
    private:
        void* data;
        std::size_t data_size;

    public:

        fileBuff();
        fileBuff(void* temp_data, size_t temp_size);
        // fileBuff(fileBuff* temp);
        ~fileBuff();

        void set(void* temp_data, size_t temp_size);
        inline void* getdata(){return data;}
        inline std::size_t getdatasize(){return data_size;}
        
        std::size_t Read( std::string filePath);
        std::size_t Write( std::string filePath);
        std::vector<std::string>  list();
        void compress(std::vector<fileBuff*> &input);
        std::vector<fileBuff*> decompress();
};

#endif // __FILEBUFF_H_INCLUDED__