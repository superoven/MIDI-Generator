#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_num_threads() 0
#define omp_get_thread_num() 0
#endif

int main(int argc, const char **argv)
{
  std::cout << "I am the main thread.\n";
  int i = 0;

#pragma omp parallel for
  for (int i = 0; i < 10; i++) {
    std::cout << "i = " << i << std::endl;
  }
  
  std::cout << "Here I am, back to the main thread.\n";
  std::cout << "i = " << i << std::endl;

  return 0;
}
