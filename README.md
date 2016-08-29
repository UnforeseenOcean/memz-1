# memz

A rewritten version of [Leurak's MEMZ Trojan](https://github.com/Leurak/MEMZ), with cleaner code and structure.

If you #define __HARMFUL__, this program WILL damage your computer.

I am not responsible for any damage made using this program.

Differences from the original:

* New code structure to make it easier to read
* You can compile and link with MinGW or Clang
* There is no GUI sandbox option
* Simpler payload delay logic (affects acceleration of payload cicle)
* Assembly/Images/Songs are not included as resources (only as binary already in the code)
