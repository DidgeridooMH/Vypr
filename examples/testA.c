(var + var)

    struct Alpha
{
  int b;
  int c;
};

typedef struct Alpha A;
typedef A B;

int main()
{
  B b;
  return 0;
}