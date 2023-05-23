# Overlord

* A simple PoC demonstrating BYOVD by abusing the Process Hacker driver to terminate other processes, The sample has been sourced from [loldrivers](https://www.loldrivers.io/drivers/edd29861-6984-4dbe-8e7c-22e9b6cf68d0/)

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/7f40c0ed-dc5d-45a3-9ff4-e1f1610b5c24)

* The driver checks if the requestor has the SeDebug privilege enabled, We can bypass this check by enabling the SeDebug privilege

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/b4263b87-468b-4a50-a80b-1e25262aeaab)

* Then, it copies the handle and other passed parameters, The specific parameters used depend on the IOCTL type

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/47f840ce-a8f8-4af8-a895-fd6deb41fcb4)

* list of IOCTL https://processhacker.sourceforge.io/doc/kphapi_8h.html make it easy to abuse

![image](https://github.com/ZeroMemoryEx/Overlord/assets/60795188/0ebfcb3b-6bda-4c12-aca5-bc27c3749d0f)
