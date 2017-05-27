#ifndef _FILE_MGR_HPP
#define _FILE_MGR_HPP
// module for file manager

#include <map>
#include <fstream>
#include <vector>

namespace iomgr{
    class filemgr_t{
        private:
            std::map<std::string, std::fstream> _fpdict;
            // format controller
            size_t _width = 16;
            // helper
            bool name_exist(const std::string& name);
        public:
            // constructor
            filemgr_t();
            // getter
            size_t size(void);
            // modifier
            void open(const std::string& name, std::fstream::openmode mode);
            void close(const std::string& name);
            // reader/writer for binary files
            void read(const std::string& name, char* offset, size_t len);
            void write(const std::string& name, char* offset, size_t len);
            // reader/writer for text files
            std::string getline(const std::string& name);
            void newline(const std::string& name);
            void writeline(const std::string& name, const std::string& text);
            void drawline(const std::string& name, char c, size_t len = 32);
    };
};

#endif
