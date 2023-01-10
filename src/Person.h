#ifndef HPC_ASSIGNMENT_PERSON_H
#define HPC_ASSIGNMENT_PERSON_H

#include <string>

using namespace std;

class Person {
public:
    Person();
    virtual ~Person();
    void say(string msg);
};


#endif //HPC_ASSIGNMENT_PERSON_H
