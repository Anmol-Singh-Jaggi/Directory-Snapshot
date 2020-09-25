import argparse
import os


def check_positive(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return ivalue


def prepare_args_parser():
    '''
    Construct and return the argument parser object.
    '''
    description = 'Directory-Snapshot: A tool to create a snapshot'
    description += ' ( poor man\'s backup ) of a directory.\n'
    epilog = 'Copyright (c) 2020 Anmol Singh Jaggi'
    epilog += '\nhttps://anmol-singh-jaggi.github.io'
    epilog += '\nMIT License'
    parser = argparse.ArgumentParser(
        description=description,
        prog='dir_snap',
        epilog=epilog,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        'src-path', help='The path of the source directory.')
    parser.add_argument(
        'dest-path', help='The path of the destination directory.')
    parser.add_argument(
        '-dry',
        '--dry-run',
        action='store_true',
        help='Just estimate the size of source directory and exit')
    parser.add_argument(
        '-no-bar',
        '--hide-progress-bar',
        action='store_true',
        help='Whether to hide the progress bar or not (default false).'
             ' Will be silently ignored if `tqdm` package not found.')
    parser.add_argument(
        '-no-hid',
        '--ignore-hidden',
        action='store_true',
        help='Whether to ignore hidden files and directories or not (default false).')
    parser.add_argument(
        '-no-sym',
        '--ignore-symlinks',
        action='store_true',
        help='Whether to ignore symlinks or not (default false).')
    parser.add_argument(
        '-max-rec',
        '--max-recursion-depth',
        help='Maximum recursion depth (non-negative integer only)',
        type=check_positive,
        default=-1)

    args = parser.parse_args()
    args = vars(args)
    return args
