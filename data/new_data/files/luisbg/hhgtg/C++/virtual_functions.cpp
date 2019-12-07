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

  virtual void about_me ()
  {                             // virtual function will be overriden
    cout << "I am a person, " << age << " years old.\n";
  }

  virtual bool add_course (string s) = 0;       // implementation left to subclass

  int get_age ()
  {
    return age;
  }

  int get_id ()
  {
    return id;
  }

  virtual ~ Person () {
    cout << "Deleting a person." << endl;
    delete name;                // free any memory allocated within the class
  }
};

class Student:public Person
{
  string course;                // private members

public:
  Student (int a)             // constructor with parent constructor
  : Person (a)
  { }

  void about_me ()
  {                             // override parent's function
    cout << "I am a student in " << course << " with age " << get_age ()
        << ".\n";      // Person::get_age()
  }

  string get_course ()
  {
    return course;
  }

  bool add_course (string s)
  {
    cout << "Added course " << s << " to student." << endl;
    course = s;
    return true;
  }

  ~Student () {
    cout << "Deleting a student." << endl;
  }
};


int
main ()
{
  Person *p = new Student (16);
  p->add_course ("History");
  p->about_me ();               // prints "I am a student."
  delete p;                     // Important! Make sure to delete the allocated memory

  return 0;
}
