from collections import deque


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


def get_num_files_in_dir_recursive(root_dir_path: Path = "."):
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
            try:
                for entry in top.iterdir():
                    bfsq.append(entry)
            except Exception as e:
                print(f"Error while listing directory contents of '{top}': {e}")
    return num_files


def append_text_to_file(path: Path, text: str):
    with path.open("a") as fp:
        fp.write(text)

