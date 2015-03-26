# Directory Snapshot
Creates a ( recursive ) backup of a directory in the form of interlinked HTML files, structured in the same form as the input directory.  
 >**It does not take a backup of the contents of the directory.**
 
It just stores the names and sizes of all the files and folders contained in the input directory so that in case of a data loss, we atleast know what all needs to be recovered. ( because, why not ?!! XD )  

*It can be thought of as a hyperlinked version of the [`dir /s`][1] or [`tree /f`][2] commands.*

Compiled using Visual Studio 2012, so works only on Windows.

**To-Do** -:

 - Make it cross - platform using Boost.
 - Store the hash of the file along with the size, using a suitable hash algorithm.
 - Add more extensive error-handling.

[1]:https://technet.microsoft.com/en-in/library/cc755121.aspx
[2]:https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/tree.mspx?mfr=true
