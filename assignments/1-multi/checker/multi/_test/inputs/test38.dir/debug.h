#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifndef DEBUG_STR
#define DEBUG_STR "debuging"
#endif

#if DEBUG
#define debug fprintf(stderr, DEBUG_STR "\n")
#else
#define debug
#endif

#endif /* _DEBUG_H_ */
