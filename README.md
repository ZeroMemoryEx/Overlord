# Overlord

* A simple PoC demonstrating BYOVD by abusing the Process Hacker driver to terminate other processes, The sample has been sourced from [loldrivers](https://www.loldrivers.io/drivers/edd29861-6984-4dbe-8e7c-22e9b6cf68d0/)

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/b8a8cea9-7c3c-4f0f-a9dd-114dc71f02f1)

* The driver checks if the requestor has the SeDebug privilege enabled, We can bypass this check by enabling the SeDebug privilege

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/52261009-1c46-4d37-926f-4438c0b31533)

* Then, it copies the handle and other passed parameters, The specific parameters used depend on the IOCTL type

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/886984b9-6142-4df3-85b4-d1996855d6e9)

* list of process hacker IOCTLs [Process Hacker IOCTLs List ](https://processhacker.sourceforge.io/doc/kphapi_8h.html) making it easy to abuse

