#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

/**
 * @brief <h1> Get the free ran in bytes</h1>
 * Returns the free ran, the ram not allocated by the heap or the stack
 * in an embedded system, in bytes.
 * Usefull for determining if a memory leak has occurred.
 * 
 * @examplecode
 * @code{.cpp}
 * Serial.println(freeMemory());
 * @endcode 
 *
 * @return int Bytes in the ram not allocated.
 * 
 * @author themicp
 */
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
