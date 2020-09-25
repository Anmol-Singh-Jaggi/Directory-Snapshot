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

```
dir_snap [-h] [-dry] [-no-bar] [-no-hid] [-no-sym] [-max-rec MAX_RECURSION_DEPTH] src-path dest-path

positional arguments:
  src-path              The path of the source directory.
  dest-path             The path of the destination directory.

optional arguments:
  -h, --help            show this help message and exit
  -dry, --dry-run       Just estimate the size of source directory and exit
  -no-bar, --hide-progress-bar
                        Whether to hide the progress bar or not (default
                        false). Will be silently ignored if `tqdm` package not
                        found.
  -no-hid, --ignore-hidden
                        Whether to ignore hidden files and directories or not
                        (default false).
  -no-sym, --ignore-symlinks
                        Whether to ignore symlinks or not (default false).
  -max-rec MAX_RECURSION_DEPTH, --max-recursion-depth MAX_RECURSION_DEPTH
                        Maximum recursion depth (non-negative integer only)
```

 - Example:  
   `python3 dir_snap.py /Users/ajaggi/projects /Users/ajaggi/Desktop/snapshots --ignore-symlinks -no-hid -max-rec=5`

## To-Do
 - Debug progress bar populating way too soon for projects dir.
 - Use `logging` module instead of `print` statements.
 - Log full exception stacktrace rather than just the message.
 - Test for unpredictable IO.
 - Add demo.
 - Publish to pypi.
 - Add test cases.
 - Add code coverage.
 - Setup `tox` and `Travis CI`.
 - Add benchmarks.
 - Explore parallel processing.
 - ~~Implement `dry-run` option.~~
 - ~~Implement `no-hid` option.~~
 - ~~Implement `no-sym` option.~~
 - ~~Implement `max-rec` option.~~

[1]:https://technet.microsoft.com/en-in/library/cc755121.aspx
[2]:https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/tree.mspx?mfr=true
