#ifndef __ratetype_h__
#define __ratetype_h__



#endif // !__ratetype_h__

#define _1000_HZ 1000
#define _500_HZ  500
#define _100_HZ  100   
#define _50_HZ   50
#define _25_HZ   25
#define _10_HZ   10

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (_1000_HZ / RATE_HZ)) == 0)

