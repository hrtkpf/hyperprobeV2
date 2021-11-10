#define cpuid(func, ax, bx, cx, dx)\
        __asm__ __volatile__ ("cpuid":\
        "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func), "c" (0x0));  // ECX is always set to 0x0 before cpuid instruction

/* When EAX=1 */
#define ECX_BIT_VMX    5
#define ECX_BIT_X2APIC    21
#define ECX_BIT_POPCNT    23

/* When EAX=40000000h */
#define EDX_SIGNATURE    0x4d

/* When EAX=80000001h */
#define ECX_BIT_SVM    2
#define EDX_BIT_MTRR    12
#define EDX_BIT_RDTSCP    27
