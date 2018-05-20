#include <bits/stdc++.h>
using namespace std;
class Geeks
{
    public:
    int id;
     
    //Definition for Destructor
    ~Geeks()
    {
        cout << "Destructor called for id: " << id <<endl; 
    }

    class Books {
    };
};
 
class Geeks3 {
  public:
  int id;
};

/*
 *
 * class BlockComment
 *
 */
//class Comment {}
int main() 
  {
     
    Geeks obj1;
    obj1.id=7;
    int i = 0;
    while ( i < 5 )
    {
        Geeks obj2;
        obj2.id=i;
        i++;
         
    } // Scope for obj2 ends here
    scope
    return 0;
  } // Scope for obj1 ends here
