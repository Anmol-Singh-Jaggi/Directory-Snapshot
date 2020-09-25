template_html_header = r'''
<!DOCTYPE html>
<html>
<head>
<meta charset=UTF-8>
<title> {filename} </title>
<style type=text/css>
    tr:nth-child(even) {{ background-color: #FFFFFF; }}
    tr:nth-child(odd) {{ background-color: #F1F1F1; }}
</style>
</head>
<body>
'''


template_files_table_header = r'''
<h1> Files </h1>
<table>
'''

template_files_table_row = r'''
  <tr>
    <td> {filename} </td>
    <td> {filesize} </td>
  </tr>
'''[1:]


template_dirs_table_header = r'''
<h1> Directories </h1>
<table>
'''


template_dirs_table_row = r'''
  <tr>
    <td> <a href="{href}.html"> {dirname} </a> </td>
    <td> {dirsize} </td>
  </tr>
'''[1:]


template_symlinks_table_header = r'''
<h1> Symlinks </h1>
<table>
'''


template_symlinks_table_row = r'''
  <tr>
    <td> {filename} </td>
    <td> {filename_resolved} </td>
  </tr>
'''[1:]


template_table_footer = r'''
<br> <h3> Total directory size = {total_dir_size} </h3> <br>
</body>
</html>
'''


template_table_close= "</table>\n"