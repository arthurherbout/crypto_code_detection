//===---------------------- LLVM C Backend test file ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This code tests to see that the CBE can handle declaring
// and returning an unsigned char.
// *TW
//
//===----------------------------------------------------------------------===//

int main() {
  unsigned char a = 'A';
  int ia = 0;

  ia = a;
  ia -= 59;

  return ia;
}
