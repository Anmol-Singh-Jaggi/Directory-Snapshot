import templates
from utils import round_file_size_to_human_friendly_units, escape_html_special_chars, append_text_to_file


class TableEntryWriter:
    def __init__(self, progress_bar):
        self.progress_bar = progress_bar

    def write_file(self, html_file, file_entry):
        filesize = file_entry.stat().st_size
        filesize_rounded = round_file_size_to_human_friendly_units(filesize)
        filename_escaped = escape_html_special_chars(file_entry.name)
        table_row = templates.template_files_table_row.format(filename=filename_escaped, filesize=filesize_rounded)
        append_text_to_file(html_file, table_row)
        self.progress_bar.update() if self.progress_bar else None
        return filesize

    def write_dir(self, html_file, dir_entry, dir_size):
        dir_size_readable = round_file_size_to_human_friendly_units(dir_size)
        dirname_escaped = escape_html_special_chars(dir_entry.name)
        dirname_escaped = escape_html_special_chars(dir_entry.name, True)
        href = dirname_escaped + "/" + dirname_escaped
        table_row = templates.template_dirs_table_row.format(href=href, dirname=dirname_escaped, dirsize=dir_size_readable)
        append_text_to_file(html_file, table_row)

    def write_symlink(self, html_file, symlink_entry):
        filename_escaped = escape_html_special_chars(symlink_entry.name)
        file_resolved = symlink_entry.resolve().name
        table_row = templates.template_symlinks_table_row.format(filename=filename_escaped, filename_resolved=file_resolved)
        append_text_to_file(html_file, table_row)
        self.progress_bar.update() if self.progress_bar else None
