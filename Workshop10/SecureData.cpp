// Workshop 10 - Multi-Threading
// SecureData.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include "SecureData.h"
#define VERBOSE 0
using namespace std::placeholders;

namespace w10 {
  /*
  void converter(char* t, char key, int n, const Cryptor& c) {
    for (int i = 0; i < n; i++)
      t[i] = c(t[i], key);
  }
  */
  // s: start point of text in memory
  std::mutex coutLock;
  void converter(char* t, char key, int s, int e, const Cryptor& c) {

    coutLock.lock();
    if(VERBOSE) std::cout << "Thread start: " << s << ", end: " << e << std::endl;
    for (int i = s; i < e && t[i] != '\0'; i++) {
      t[i] = c(t[i], key);
    }
    coutLock.unlock();
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

    if (VERBOSE) std::cout << "In Memory before decoding-->"<< text << "<--" << std::endl;

    std::cout << "\n" << nbytes << " bytes copied from text "
      << file << " into memory (null byte added)\n";
    encoded = false;

    // encode using key
    code(key);
    std::cout << "Data encrypted in memory\n";
    if (VERBOSE) std::cout << "Backup: \n" << text << "\n";
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

    auto bindFunc = std::bind (converter, text, key, _1, _2, Cryptor());

    std::thread* th1 = new std::thread(bindFunc, 0,   100);
    std::thread* th2 = new std::thread(bindFunc, 101, 200);
    std::thread* th3 = new std::thread(bindFunc, 201, 300);
    std::thread* th4 = new std::thread(bindFunc, 301, 400);
    std::thread* th5 = new std::thread(bindFunc, 401, 500);
    std::thread* th6 = new std::thread(bindFunc, 501, 600);
    std::thread* th7 = new std::thread(bindFunc, 601, 700);
    std::thread* th8 = new std::thread(bindFunc, 701, 800);
    std::thread* th9 = new std::thread(bindFunc, 801, 999);

    th1->join(); th2->join(); th3->join(); th4->join(); th5->join(); 
    th6->join(); th7->join(); th8->join(); th9->join();

    delete th1; delete th2; delete th3; delete th4; delete th5;
    delete th6; delete th7; delete th8; delete th9;
    
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
#if 1
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
    {
      fbin >> text[i++];
    }
    text[--i] = '\0';

    if (VERBOSE) std::cout << "Restore Test: \n" << text << "\n";

    fbin.close();

    std::cout << "\n" << nbytes + 1 << " bytes copied from binary file "
      << file << " into memory (null byte included)\n";
    encoded = true;

    // decode using key
    code(key);
#endif
    std::cout << "Data decrypted in memory\n\n";
  }

  std::ostream& operator<<(std::ostream& os, const SecureData& sd) {
    sd.display(os);
    return os;
  }

}
