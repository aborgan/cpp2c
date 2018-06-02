#include <stdio.h>
using namespace std;
class Geeks
{
    public:
    int id;
     
    void testMethod(int test, int *test2, char test3) {
      int x = 5;
      int y = 6;
      int z = x + y;
      printf("z: %d\n", z);
    }

    void testMethod2(int hello) {
      printf("testing 1 2 3 testing\n");
    }

    class Books {
      public:
      int test() {
        printf("skip me first time\n");
      }
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
