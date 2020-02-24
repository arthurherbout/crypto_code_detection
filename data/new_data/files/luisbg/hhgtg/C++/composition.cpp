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

  int get_age ()
  {
    return age;
  }

  int get_id ()
  {
    return id;
  }

  ~Person () {
    delete name;                // free any memory allocated within the class
  }
};

class Student
{
  Person person;
  int course;                   // private members

public:
  // constructor with member initialization
  Student(int a, int c): person(a), course(c)
  {}

  int get_age ()
  {
    return person.get_age();
  }

  int get_id ()
  {
    return person.get_id();
  }

  void about_me ()
  {                             // override parent's function
    cout << "I am a " << get_age () << " year old student in grade " << course
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

  ~Student()
  {}
};


int
main ()
{
  Student *p = new Student (16, 10);
  p->about_me ();               // prints "I am a student."
  cout << "id: " << p->get_id () << "\n";

  p->set_course (11);
  cout << "Passed the class. Now at grade " << p->get_course() << "\n";

  delete p;                     // Important! Make sure to delete the allocated memory

  return 0;
}
