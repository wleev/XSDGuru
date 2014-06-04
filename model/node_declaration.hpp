#ifndef NODE_DECLARATION_HPP_
#define NODE_DECLARATION_HPP_

#include <string>

class NodeDeclaration
{
public:
   NodeDeclaration(std::string name__) : name(name__){};
   virtual ~NodeDeclaration(){};

   std::string getName(){ return name;}
   void setName(std::string name_){ name = name_;}

   std::string getTypeName(){ return typeName; }
   void setTypeName(std::string typeName__){ typeName = typeName__;}
protected:
   std::string name;
   //optional typename property
   std::string typeName;
};

#endif
