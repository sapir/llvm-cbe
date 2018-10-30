//===---------------------- LLVM C Backend test file ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This code tests to see that the CBE will properly use a NULL pointer and
// confirm that it is equal to zero.
// *TW
//
//===----------------------------------------------------------------------===//
#include <stddef.h>

int main() {
  int *ptr = NULL;
  if (ptr == 0) {
    return 6;
  }
  return 1;
}
