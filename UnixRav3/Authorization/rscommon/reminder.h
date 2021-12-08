#ifndef __REMINDER_H__
#define __REMINDER_H__

#define chSTR2(x)		#x
#define chSTR(x)		chSTR2(x)
#define reminder(desc)	message(__FILE__ "(" chSTR(__LINE__) ") : " #desc)

// usage:
// #pragma reminder(Fix this later)

#endif // __REMINDER_H__
