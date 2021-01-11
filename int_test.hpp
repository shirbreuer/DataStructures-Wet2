#ifndef INT_H
#define INT_H

class intTest
{
private:
    int key;

public:
    intTest(int num): key(num) {}
    ~intTest();
    const int getKey() { return this->key; }
};

#endif