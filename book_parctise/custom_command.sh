#!/bin/bash
echo "-- call custom command"

# get variable with meaning name
res_name=$1
source_path=$2
runtime_path=$3
echo "-- resource folder name \"${res_name%}\""
echo "-- source path \"${source_path}\""
echo "-- runtime path \"${runtime_path}\""

# get orgin and target resource path
origin_res_path=${source_path}/${res_name}
target_res_path=${runtime_path}/${res_name}
echo "-- origin resource path \"${origin_res_path}\""
echo "-- target resource path \"${target_res_path}\""

# remove old resource folder
if [ -d ${target_res_path} ]; then
	rm -drf ${target_res_path}
	echo "-- remove old resouce folder \"${target_res_path}\""
fi

# copy resource folder with content into target
cp -R ${origin_res_path} ${target_res_path}

## read