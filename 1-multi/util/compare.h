#ifndef __COMPARE_H_
#define __COMPARE_H_

/**
  * Function to compare two integer values (priorities) for inserting
  * in a priority queue
  * @param prio1 - first priority (first value)
  * @param prio2 - second priority (second value)
  * @return Value higher than 0 if prio1 > prio2, 
  * 		lower than 0 if prio1 < prio2
  * 		equal to 0 if prio1 == prio2
  */
int compare(int prio1, int prio2);

#endif
