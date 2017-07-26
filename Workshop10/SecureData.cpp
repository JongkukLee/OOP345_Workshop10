// Workshop 10 - Multi-Threading
// SecureData.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "SecureData.h"

namespace w10 {

  void converter(char* t, char key, int n, const Cryptor& c) {
    for (int i = 0; i < n; i++)
      t[i] = c(t[i], key);
  }

  SecureData::SecureData(const char* file, char key) {
    // open text file
    std::fstream input(file, std::ios::in);
    if (!input)
      throw std::string("\n***Failed to open file ") +
      std::string(file) + std::string(" ***\n");

    // copy from file into memory
    nbytes = 0;
    input >> std::noskipws;
    while (input.good()) {
      char c;
      input >> c;
      nbytes++;
    }
    nbytes--;
    input.clear();
    input.seekg(0, std::ios::beg);
    text = new char[nbytes + 1];

    int i = 0;
    while (input.good())
      input >> text[i++];
    text[--i] = '\0';
    std::cout << "\n" << nbytes << " bytes copied from text "
      << file << " into memory (null byte added)\n";
    encoded = false;

    // encode using key
    code(key);
    std::cout << "Data encrypted in memory\n";
    std::cout << "Backup: \n" << text << "\n";

  }

  SecureData::~SecureData() {
    delete[] text;
  }

  void SecureData::display(std::ostream& os) const {
    if (text && !encoded)
      os << text << std::endl;
    else if (encoded)
      throw std::string("\n***Data is encoded***\n");
    else
      throw std::string("\n***No data stored***\n");
  }

  void SecureData::code(char key) {
    converter(text, key, nbytes, Cryptor());
    encoded = !encoded;
  }

  void SecureData::backup(const char* file) {
    if (!text)
      throw std::string("\n***No data stored***\n");
    else if (!encoded)
      throw std::string("\n***Data is not encoded***\n");
    else {

      // Reference: http://www.cplusplus.com/forum/beginner/13924/
      // open binary file
      std::ofstream fbin(file, std::ios::binary);

      if (!fbin)
      {
        throw std::string("Could not open file ") + file;
      }
      // write binary file 
      fbin.write(text, nbytes);
      fbin.close();
    }
  }

  void SecureData::restore(const char* file, char key) {
    delete[] text;
    text = nullptr;

    // open binary file
    std::ifstream fbin(file, std::ios::binary);

    if (!fbin)
    {
      throw std::string("Could not open file ") + file;
    } 
    // allocate memory here
    text = new char[nbytes + 1];
    
    // read binary file here
    int i = 0;
    while (fbin.good())
      fbin >> text[i++];
    text[--i] = '\0';

    std::cout << "Restore Test: \n" << text << "\n";

    fbin.close();

    std::cout << "\n" << nbytes + 1 << " bytes copied from binary file "
      << file << " into memory (null byte included)\n";
    encoded = true;

    // decode using key
    code(key);
    std::cout << "Data decrypted in memory\n\n";
  }

  std::ostream& operator<<(std::ostream& os, const SecureData& sd) {
    sd.display(os);
    return os;
  }

}
