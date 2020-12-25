echo -- call custom command

: get variable with meaning name
set res_name=%1
set source_path=%2
set runtime_path=%3
echo -- resource folder name "%res_name%"
echo -- source path "%source_path%"
echo -- runtime path "%runtime_path%"

: replace / to \
set "source_path=%source_path:/=\%"
set "runtime_path=%runtime_path:/=\%"

: debug and release folder
set runtime_path_debug=%runtime_path%\Debug
set runtime_path_release=%runtime_path%\Release

: get orgin and target resource path
set origin_res_path=%source_path%\%res_name%
set target_res_path=%runtime_path%\%res_name%
set target_res_path_debug=%runtime_path_debug%\%res_name%
set target_res_path_release=%runtime_path_release%\%res_name%

: remove old resource files
if exist "%target_res_path%" (
	echo -- remove old resouce folder "%target_res_path%"
	rmdir /s/q "%target_res_path%"
)
if exist "%target_res_path_debug%" (
	echo -- remove old debug resouce folder "%target_res_path_debug%"
	rmdir /s/q "%target_res_path_debug%"
)
if exist "%target_res_path_release%" (
	echo -- remove old release resouce folder "%target_res_path_release%"
	rmdir /s/q "%target_res_path_release%"
)

: copy resource folder with content into target
echo -- start copy resource files
: for common
md %target_res_path%
XCOPY /E %origin_res_path% %target_res_path%
: for debug
if exist "%runtime_path_debug%" (
	md %target_res_path_debug%
	XCOPY /E %origin_res_path% %target_res_path_debug%
)
: for release
if exist "%runtime_path_release%" (
	md %target_res_path_release%
	XCOPY /E %origin_res_path% %target_res_path_release%
)
echo -- complete copy resource files