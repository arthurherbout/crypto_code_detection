#include <iostream>
using namespace std;

class Person
{
  int id;                       // all members are private by default
  int age;
  char *name;

public:
  Person (int a)
  {                             // constructor
    age = a;
  }

  void about_me ()
  {
    cout << "I am a person, " << age << " years old.\n";
  }

  int get_id ();
  int get_age ();

  ~Person () {
    delete name;                // free any memory allocated within the class
  }
};

class Student:public Person
{
  int course;                   // private members

public:
  Student (int a, int c)        // constructor with parent constructor
  : Person (a)
  {
    course = c;
  }

  void about_me ()
  {                             // override parent's function
    cout << "I am a student in grade " << course << " with age " << get_age ()
        << ".\n";        // Person::get_age()
  }

  int get_course ()
  {
    return course;
  }

  void set_course (int c)
  {
    course = c;
  }
};

int Person::get_id ()
{
  return id;
}

int Person:: get_age ()
{
  return age;
}


int
main ()
{
  Student *p = new Student (16, 10);
  p->about_me ();               // prints "I am a student."
  cout << "id: " << p->get_id () << "\n";
  delete p;                     // Important! Make sure to delete the allocated memory

  return 0;
}
