# Directory Snapshot

![MIT License](https://img.shields.io/badge/license-MIT-green.svg)
![Blazing fast](https://img.shields.io/badge/speed-blazing%20%F0%9F%94%A5-brightgreen.svg)
[![Say thanks](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://saythanks.io/to/Anmol-Singh-Jaggi)

- A recursive directory listing program that produces a set of interlinked HTML files, structured in the same form as the input directory.
- Rather than taking a backup of the **contents** of a directory; instead it just stores the **metadata** (name, size etc.) of all the contents in a directory.  
- It can be useful when you do not have enough space for a full backup, and your data consists of files which could be obtained easily again from another source after a disk failure (for example, software installation files).  
  *It can be thought of as a hyperlinked version of the [`dir /s`][1] or [`tree /f`][2] commands.*

## Installation
 - Requires Python 3.7+.
 - `pip install tqdm` (Required only if you want progress bar functionality).

## Demo
- TODO

## Usage

    dir_snap [-h] [-dry] [-no-bar] [-ignore-hidden] []
               source-path dest-path

    positional arguments:
    left-path             The path of the left(source) directory.
    right-path            The path of the right(destination) directory.

    optional arguments:
      -h, --help            show this help message and exit
      -dry, --dry-run       Just estimate the size of the input directory.
      -no-bar, --hide-progress-bar
                            Whether to hide the progress bar or not (default false).  
                            Will be silently ignored if `tqdm` dependency not found.
      -no-hid, --ignore-hidden
                            Whether to ignore hidden files and directories or not (default false).
      -no-sym, --ignore-symlinks
                            Whether to ignore symlinks or not (default false).
      -max-rec, --max-recursion
                            The max recursion depth (default infinite).

## ToDo
 - Implement `dry-run` option.
 - Implement `no-hid` option.
 - Implement `no-sym` option.
 - Implement `max-rec` option.
 - Test for unpredictable IO.
 - Add test cases.
 - Add code coverage.
 - Setup `tox` and `Travis CI`.
 - Explore parallel processing.
 - Add benchmarks.
 - Add demo.
 - Use `logging` module instead of `print` statements.
 - Publish to pypi.


[1]:https://technet.microsoft.com/en-in/library/cc755121.aspx
[2]:https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/tree.mspx?mfr=true
