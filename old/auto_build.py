import os
import sys

# get argv
input_name = sys.argv[0]
cmake_build_project_type = sys.argv[1]
cmake_build_type = sys.argv[2]
build_folder = "/build"
cmake_build_command = "cmake .. -G \"" + cmake_build_project_type + "\"" + " -DCMAKE_BUILD_TYPE=" + cmake_build_type
cmake_compile_command = "cmake --build . --config " + cmake_build_type

# get directory and path string
path = os.path.abspath(input_name)
cur_dir = os.path.dirname(path)

print " === Project directory: ", cur_dir

# ensure build folder exists
os.chdir(cur_dir)
build_path = cur_dir + build_folder
if not os.path.exists(build_path):
	os.mkdir(build_path, 0755)

# cmake create project
print " === Create project"
os.chdir(build_path)
os.system(cmake_build_command)

# cmake compile project
print " === compile project"
os.system(cmake_compile_command)

# wait exit
raw_input("Press any key to exit...")
