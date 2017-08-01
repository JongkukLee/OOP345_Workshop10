// OOP345 Workshop 10: Multi-Threading
// File SecureData.h
// Version 1.0
// Date 2017-08-04
// Author Jongkuk Lee ( jlee465@myseneca.ca, 127730158 )
// Description
//  The SecureData class holds text in encoded form along with 
//  the number of bytes in the text including the null terminator.
//  The Cryptor function object holds the encryption / decryption 
//  logic used by the SecureData class.
//
// Revision History
///////////////////////////////////////////////////////////
// Name     Date    Reason
//
///////////////////////////////////////////////////////////
// Workshop 10 - Multi-Threading
// SecureData.h

#ifndef W10_SECUREDATA_H
#define W10_SECUREDATA_H

#include <iostream>

namespace w10 {

  class Cryptor {
  public:
    char operator()(char in, char key) const { return in ^ key; }
  };
  //void converter(char*, char, int, const Cryptor&);

  void converter(char*, char, int, int, const Cryptor&);

  class SecureData {
    char* text;
    int nbytes;
    bool encoded;
    void code(char);
  public:
    SecureData(const char*, char);
    SecureData(const SecureData&) = delete;
    SecureData& operator=(const SecureData&) = delete;
    ~SecureData();
    void display(std::ostream&) const;
    void backup(const char*);
    void restore(const char*, char);
  };
  std::ostream& operator<<(std::ostream&, const SecureData&);
}
#endif