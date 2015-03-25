Creates a ( recursive ) backup of a directory in the form of interlinked HTML files, structured in the same form as the input directory.  
 >**It does not take backup of the contents of the directory.**
 
It just stores the names and sizes of all the files and folders contained in the input directory so that in case of a data loss, we atleast know what all needs to be recovered. ( because, why not XD )  

*Can be thought of as an HTML-structured version of `dir /s` command.*

Compiled using Visual Studio 2012, so works only on Windows.

**To-Do** -:

 - Create a portable version using Boost.
 - Store the hash of the file along with the size, using a suitable hash algorithm.
 - Handle symlinks and other such non-file/folders.

