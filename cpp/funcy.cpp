#include <stdio.h>
#include <stdlib.h>

class base
{
  public:
    base();

    virtual void someFunc() = 0;
};

class first : public base
{
  public:
    first();

    void someFunc();
};

class second : public base
{
  public:
    second();

    void someFunc();
};

first::first() : base()
{
}

void first::someFunc()
{
    printf("first called func\n");
}

second::second() : base()
{
}

void second::someFunc()
{
    printf("second called func\n");
}

int main(int argc, char **argv)
{
    first *f = new first();

    second *s = new second();

    void (base::*func)(void);

    func = &base::someFunc;

    void *p = NULL;

    p = f;

    ((base *)p)->func();

    return 1;
}
