#include <iostream>
using namespace std;

class BookShelf
{
  int id;
  int num_books;                // number of books in a shelf
  // list of books would go here

public:
  BookShelf (int d, int num)
  {                             // constructor
    id = d;
    num_books = num;
  }

  int get_id ()
  {
    return id;
  }

  int get_num_books ()
  {
    return num_books;
  }

  int operator+ (BookShelf other);
  int operator- (BookShelf * other);

  ~BookShelf () { }             // destructor
};

int
BookShelf::operator+ (BookShelf other)
{
  return num_books + other.get_num_books ();
}

int
BookShelf::operator- (BookShelf * other)
{
  return num_books - other->get_num_books ();
}

int
main ()
{
  BookShelf *a = new BookShelf (0, 10);
  BookShelf *b = new BookShelf (1, 5);

  cout << "id: " << a->get_id () << " books: " << a->get_num_books () << endl;
  cout << "id: " << b->get_id () << " books: " << b->get_num_books () << endl;

  cout << "the two bookshelves have: " << *a + *b << endl;
  cout << "the first minus the second: " << *a - b << endl;

  delete a;
  delete b;

  return 0;
}
