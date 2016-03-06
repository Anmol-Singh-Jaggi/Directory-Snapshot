# Directory Snapshot
A recursive directory listing program that produces a set of interlinked HTML files, structured in the same form as the input directory.

It does not take a backup of the contents of a directory; just stores the metadata (name, size etc.) of all the files and folders contained in the directory recursively.  
It can be useful when you do not have enough space for a full backup, and your data consists of files which could be obtained easily again from another source after a disk failure (for example, software installation files).  

*It can be thought of as a hyperlinked version of the [`dir /s`][1] or [`tree /f`][2] commands.*

**Requirements:**
 - A compiler supporting C++11
 - The [Boost.Filesystem][4] library

**Installation:**
 - Before compiling this, you need to [build the Boost.Filesystem object-library][4]  
 - Execute *`INSTALL`*
 
**Usage:**
 - An example has been included to demonstrate its functioning  
 - For running the program on the example yourself, execute *`RUN`*

**TODO:**
 - Reorganize the project by splitting into multiple files.

[1]:https://technet.microsoft.com/en-in/library/cc755121.aspx
[2]:https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/tree.mspx?mfr=true
[3]:http://www.boost.org/doc/libs/1_57_0/libs/filesystem/doc/index.htm
[4]:http://www.boost.org/doc/libs/1_57_0/libs/filesystem/doc/index.htm#Building
