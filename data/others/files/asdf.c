/*******************************************************************************
  Dummy library
********************************************************************************
  Alfredo Canziani, Nov 14
*******************************************************************************/

// Dummy function definition
void dummy(int* ptrFromLua, int size) {

  for (int i = 0; i < size; i++)
    ptrFromLua[i] = i + 1;

  return;

}
