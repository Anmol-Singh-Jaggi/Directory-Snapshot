# Directory Snapshot
Creates a ( recursive ) backup of a directory in the form of interlinked HTML files, structured in the same form as the input directory.  
 >**It does not take a backup of the contents of the directory.**
 
It just stores the names and sizes of all the files and folders contained in a directory, so that in case of a data loss, we atleast know what all needs to be recovered. ( because, why not ?!! XD )  

*It can be thought of as a hyperlinked version of the [`dir /s`][1] or [`tree /f`][2] commands.*

**Requirements -:**
 - A compiler supporting C++ 11
 - The [Boost][3] C++ library.

**An example has been included to demonstrate its functioning.**  
 > For running the input example yourself, first delete the sample output -  *example/output/coding*, and then execute the program by entering the path to *example/input/coding* as the source directory and the path to *example/output* as the destination directory.
[1]:https://technet.microsoft.com/en-in/library/cc755121.aspx
[2]:https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/tree.mspx?mfr=true
[3]:http://www.boost.org/
