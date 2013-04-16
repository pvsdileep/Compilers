#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Address {
public:
  virtual ~Address() {}
  virtual string str() const = 0;
};

//inline means replace function call with this
inline ostream& operator<<(ostream& out, const Address& addr) {
  out << addr.str();
  return out;
}

class NameAddress : public Address {
  const string& name;
public:
  NameAddress(const string& n) : name(n) {}
  virtual string str() const {
	  //cout<<"\nin NameAddress"<<endl;
	  return name;}
};

class ConstAddress : public Address {
  const float val;
public:
  ConstAddress(float v) : val(v) {}
  virtual string str() const {
	//  cout<<"\nin ConstAddress"<<endl;
    stringstream ss;
    ss << val;
    return ss.str();
  }
};

class TempAddress : public Address {
  static int next;
   int num;
public:
  TempAddress() : num(next++) {}
  virtual string str() const {
	 // cout<<"\nin TempAddress\n";
    stringstream ss;
    ss << "t" << num;
    return ss.str();
  }
  static void reset() {next = 0;}
};    

#endif // ADDRESS_H
