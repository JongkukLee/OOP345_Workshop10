// OOP345 Workshop 10: Multi-Threading
// File w10.cpp
// Version 1.0
// Date 2017-08-04
// Author Jongkuk Lee ( jlee465@myseneca.ca, 127730158 )
// Description
//      This file is a test file for Workshop10.
//
// Revision History
///////////////////////////////////////////////////////////
// Name     Date    Reason
//
///////////////////////////////////////////////////////////
// Workshop 10 - Multi-Threading
// w10.cpp

#include <iostream>
#include <string>
#include "SecureData.h"

int main(int argc, char** argv) {

  // ex: w10 w10_text.dat encoded.dat %
  argc = 4;
  argv[0] = "w10";
  argv[1] = "w10_text.dat";
  argv[2] = "encoded.dat";
  argv[3] = "%";

  std::cout << "\nCommand Line : ";
  for (int i = 0; i < argc; i++) {
    std::cout << argv[i] << ' ';
  }
  std::cout << std::endl;
  if (argc != 4) {
    std::cerr << "\n***Incorrect number of arguments***\n";
    return 1;
  }
  char key = argv[3][0];

  try {
    w10::SecureData sd(argv[1], key);
    sd.backup(argv[2]);

    sd.restore(argv[2], key);
    std::cout << sd << std::endl;

  }
  catch (const std::string& msg) {
    std::cerr << msg << std::endl;
  }

  std::cout << "\nPress any key to continue ... ";
  std::cin.get();
}
