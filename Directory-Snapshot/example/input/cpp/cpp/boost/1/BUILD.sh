# Change these according to the Boost installation path in your system
boost_path="${HOME}/boost"
boost_object_path="${boost_path}/stage/lib"

source="main"
boost_object_files=("libboost_filesystem.a" "libboost_system.a")

for (( i = 0; i < ${#boost_object_files[@]}; i++ )); do
	boost_object_files[${i}]="${boost_object_path}/${boost_object_files[${i}]}"
done

g++ "@${HOME}/cpp-options.txt" -isystem "${boost_path}" -c "${source}.cpp" -o "${source}.o"
g++ -o "${source}" "${source}.o" "${boost_object_files[@]}"
