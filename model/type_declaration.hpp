#ifndef TYPE_DECLARATION_HPP_
#define TYPE_DECLARATION_HPP_

#include <string>

enum class ElementType
{
   INT,
   DOUBLE,
   STRING,
   BOOL,
   DATE,
   NONE
};

class TypeDeclaration
{
public:
   TypeDeclaration(std::string name__) : name(name__) {}
   virtual ~TypeDeclaration(){}

   virtual ElementType getType() = 0;
   std::string getName(){ return name;}
   
private:
   std::string name;
};




#endif


