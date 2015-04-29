
#define _WIN32_WINNT NTDDI_WINXP /* most basic version of Windows XP */
#include <Windows.h>


int main()
{
     SYSTEM_POWER_STATUS syspwr;
     if (GetSystemPowerStatus(&syspwr))
	  std::cout << "AC line active: " << syspwr.ACLineStatus == 1 ? "yes" : "no" << std::endl;
     return 0;
}
