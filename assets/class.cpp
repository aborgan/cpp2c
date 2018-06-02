class myClass{
  public:
  int x = 5;
  int y = 7;
  char letter = 'a';

  int add(int x, int y) {
    return x + y;
  }

  void printCharByChar(char *string) {
    for(int i = 0; i < strlen(string); i++) {
      printf("%c", string[i]);
    }
    printf("\n");
  }

  private:
  bool isTrue() {
    return true;
  }

  bool isFalse() {
    return false;
  }

}
