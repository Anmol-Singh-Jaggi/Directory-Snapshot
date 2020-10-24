from collections import deque
from pathlib import Path


def round_file_size_to_human_friendly_units(num_bytes):
    units = ["bytes", "KiB", "MiB", "GiB", "TiB"]
    multiplier = 1024
    ans, i = num_bytes, 0
    while ans > multiplier and i < len(units) - 1:
        ans /= multiplier
        i += 1
    return f"{ans:.2f} {units[i]}"


def escape_html_special_chars(filename, inside_href=False):
    def escape_char(ch):
        escape_map = {}
        # TODO: Test more
        # TODO: Use urllib.parse.quote() here
        # https://www.werockyourweb.com/url-escape-characters/
        escape_map['&'] = '&amp;'
        escape_map['<'] = '&lt;'
        escape_map['>'] = '&gt;'
        escape_map['"'] = '&quot;'
        escape_map["'"] = '&#39;'
        if inside_href:
            escape_map["#"] = '%23'
            escape_map["?"] = '%3F'
        return escape_map.get(ch, ch)
    return ''.join(map(escape_char, filename))


def list_dir_contents(dir_path, ignore_symlinks, ignore_hidden):
    try:
        contents: List[Path] = list(dir_path.iterdir())
        contents = filter(lambda path: not path.name.startswith('.'), contents) if ignore_hidden else contents
        contents = filter(lambda path: not path.is_symlink(), contents) if ignore_symlinks else contents
        return list(contents)
    except Exception as e:
        print(f"Error while listing directory contents of '{dir_path}': {e}")


def get_num_files_in_dir_recursive(root_dir_path: Path, ignore_symlinks, ignore_hidden):
    '''
    Returns the number of files in the input directory.
    Here "file" refers to everything (regular_files, directories, symlinks etc.).
    '''
    # Do BFS
    num_files = 0
    bfsq = deque()
    bfsq.append(root_dir_path)
    while len(bfsq) > 0:
        top = bfsq.popleft()
        num_files += 1
        if top.is_dir():
            top_contents = list_dir_contents(top, ignore_symlinks, ignore_hidden)
            if top_contents is None:
                continue
            for entry in top_contents:
                bfsq.append(entry)
    return num_files


def append_text_to_file(path: Path, text: str):
    with path.open("a") as fp:
        fp.write(text)