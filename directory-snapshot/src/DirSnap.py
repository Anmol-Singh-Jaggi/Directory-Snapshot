from pathlib import Path
import logging
from shutil import rmtree
from typing import List
import templates
from utils import escape_html_special_chars, append_text_to_file, list_dir_contents, round_file_size_to_human_friendly_units
from TableEntryWriter import TableEntryWriter


class DirSnap:
    def __init__(self, src_path, dest_path, progress_bar, ignore_hidden, ignore_symlinks, max_rec_depth):
        self.src_path = src_path
        self.dest_path = dest_path
        self.progress_bar = progress_bar
        self.ignore_hidden = ignore_hidden
        self.ignore_symlinks = ignore_symlinks
        self.max_rec_depth = max_rec_depth
        self.html_file: Path = None
        self.table_entry_writer = TableEntryWriter(progress_bar)

    def get_pwd(self):
        return self.dest_path / self.src_path.name

    def init_html_file(self):
        pwd: Path = self.get_pwd()
        if pwd.exists():
            logging.warn(f'Deleting directory "{pwd}" and its children!')
            rmtree(pwd)
        pwd.mkdir(parents=True, exist_ok=True)
        html_file: Path = pwd / (self.src_path.name + ".html")
        html_file.touch()
        self.html_file = html_file

    def write_table_header(self):
        html_header = templates.template_html_header.format(filename=escape_html_special_chars(self.src_path.name))
        self.html_file.write_text(html_header)
        return 0

    def write_table_footer(self, total_src_dir_size):
        total_src_dir_size = round_file_size_to_human_friendly_units(total_src_dir_size)
        html_footer = templates.template_table_footer.format(total_src_dir_size=total_src_dir_size)
        append_text_to_file(self.html_file, html_footer)
        return 0

    def write_files(self, files):
        total_files_size = 0
        append_text_to_file(self.html_file, templates.template_files_table_header)
        if not files:
            append_text_to_file(self.html_file, "No files found.\n")
        for entry in files:
            try:
                filesize = self.table_entry_writer.write_file(self.html_file, entry)
                total_files_size += filesize
            except Exception:
                logging.error(f"Error while processing file '{entry}'", exc_info=True)
        append_text_to_file(self.html_file, templates.template_table_close)
        return total_files_size

    def write_symlinks(self, symlinks):
        if self.ignore_symlinks:
            return 0
        append_text_to_file(self.html_file, templates.template_symlinks_table_header)
        if not symlinks:
            append_text_to_file(self.html_file, "No symlinks found.\n")
        for entry in symlinks:
            try:
                self.table_entry_writer.write_symlink(self.html_file, entry)
            except Exception:
                logging.error(f"Error while processing symlink '{entry}'", exc_info=True)
        append_text_to_file(self.html_file, templates.template_table_close)
        return 0

    def write_dirs(self, dirs):
        total_dirs_size = 0
        append_text_to_file(self.html_file, templates.template_dirs_table_header)
        if not dirs:
            append_text_to_file(self.html_file, "No subdirectories found.\n")
        elif self.max_rec_depth == 0:
            append_text_to_file(self.html_file, "Recursion depth limit reached!\n")
            dirs = []
        for entry in dirs:
            try:
                subdir_snap = DirSnap(self.src_path / entry.name, self.get_pwd(), self.progress_bar, self.ignore_hidden, self.ignore_symlinks, self.max_rec_depth - 1)
                subdir_size = subdir_snap.get_snapshot()
                total_dirs_size += subdir_size
                self.table_entry_writer.write_dir(self.html_file, entry, subdir_size)
            except Exception:
                logging.error(f"Error while processing directory '{entry}'", exc_info=True)
        append_text_to_file(self.html_file, templates.template_table_close)
        return total_dirs_size

    def write_table(self, symlinks, files, dirs):
        source_path_size = 0
        source_path_size += self.write_table_header()
        source_path_size += self.write_files(files)
        source_path_size += self.write_dirs(dirs)
        source_path_size += self.write_symlinks(symlinks)
        source_path_size += self.write_table_footer(source_path_size)
        return source_path_size

    def categorize_contents(self):
        src_path_contents: List[Path] = list_dir_contents(self.src_path, self.ignore_symlinks, self.ignore_hidden)
        if src_path_contents is None:
            return None

        # Sort, since directory iteration is not ordered on some file systems
        src_path_contents.sort()

        # Categorize the contents into separate containers depending upon their file-types
        symlinks, files, dirs = [], [], []
        for entry in src_path_contents:
            try:
                if entry.is_dir():
                    dirs.append(entry)
                elif entry.is_symlink():
                    symlinks.append(entry)
                else:
                    files.append(entry)
                    # TODO: Handle other file types also
            except Exception:
                # TODO: Output failed files in the html table somehow
                logging.error(f"Error while categorizing entry '{entry}'", exc_info=True)
        return (symlinks, files, dirs)

    def get_snapshot(self):
        src_path_contents = self.categorize_contents()
        if not src_path_contents:
            return 0
        symlinks, files, dirs = src_path_contents

        if self.src_path == DirSnap.dest_path_root:
            logging.warn(f"Original destination path reached! '{self.src_path}'")
            dest_path_root_pwd = DirSnap.dest_path_root / DirSnap.src_path_root.name
            dirs.remove(dest_path_root_pwd) if dest_path_root_pwd in dirs else None

        self.init_html_file()
        return self.write_table(symlinks, files, dirs)
