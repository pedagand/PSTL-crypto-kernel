#!/bin/bash
if [ "$#" -ne  1 ];
then
	echo "error: use process_data [data_file]"
else

	filename=$1
	keys=(128 192 256)
	blocks=(16 64 256 1024 8192)
	for keysize in ${keys[*]}
	do
		for blocksize in ${blocks[*]}
		do
			data_file=$1_$keysize"x"$blocksize
			escaped_name=$( sed  "s/_/\\\_/g" <<< "'$data_file'" ) #escape underscore
			sed -ne "s/.*$keysize bit key, $blocksize byte blocks.*(\([0-9]\+\).*)/\1/p" $filename \
			 > $data_file
			./gnuplot.sh $data_file $escaped_name
			rm $data_file
		done
	done
fi

