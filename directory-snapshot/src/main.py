#!/usr/bin/python3 -u
from pathlib import Path
import logging
try:
    from tqdm import tqdm
    tqdm_missing = False
except ImportError:
    tqdm_missing = True
    pass
from utils import get_num_files_in_dir_recursive
from args_parsing import prepare_args_parser
from DirSnap import DirSnap

logging.basicConfig(format='%(asctime)s %(levelname)-8s [%(filename)s:%(lineno)d] %(message)s', datefmt='%Y-%m-%d:%H:%M:%S', level=logging.INFO)


def main():
    args = prepare_args_parser()
    src_path = args['src-path']  # '/Users/ajaggi/Downloads'
    dest_path = args['dest-path']  # '/Users/ajaggi/Desktop/snapshots'
    src_path = Path(src_path).resolve()
    dest_path = Path(dest_path).resolve()
    logging.info('Source directory = "{}"'.format(src_path))
    logging.info('Destination directory = "{}"\n'.format(dest_path))

    if not src_path.exists():
        logging.error(f'Source path {src_path.absolute()} does not exist!')
        exit(-1)

    hide_progress_bar = args['hide_progress_bar']
    dry_run = args['dry_run']
    ignore_hidden = args['ignore_hidden']
    ignore_symlinks = args['ignore_symlinks']
    max_rec_depth = args['max_recursion_depth']

    if tqdm_missing and not hide_progress_bar:
        logging.warn('tqdm package not found! Skipping progress bar')
        hide_progress_bar = True

    progress_bar = None
    if not hide_progress_bar:
        logging.info('Precomputing directory size to populate progress bar properly...')
        source_path_size = get_num_files_in_dir_recursive(src_path, ignore_symlinks, ignore_hidden)
        logging.info(f"Source path size = {source_path_size} items")
        if not dry_run:
            progress_bar = tqdm(total=source_path_size, desc='Computing snapshot', unit=' items')

    if dry_run:
        logging.info('Exiting dry-run!')
        exit(0)

    DirSnap.src_path_root = src_path
    DirSnap.dest_path_root = dest_path
    dir_snap = DirSnap(src_path, dest_path, progress_bar, ignore_hidden, ignore_symlinks, max_rec_depth)
    dir_snap.get_snapshot()
    if progress_bar is not None:
        progress_bar.update(source_path_size - progress_bar.n)
        progress_bar.close()


if __name__ == "__main__":
    main()
