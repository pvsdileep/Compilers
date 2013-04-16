#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Label {
  static int next;
  int num;
public:
  Label() : num(next++) {}
  string str() const {
    stringstream ss;
    ss << "L" << num;
    return ss.str();
  }
};  

inline std::ostream& operator<<(std::ostream& out, const Label& label) {
  out << label.str();
  return out;
}

#endif // LABEL_H
