#!/bin/bash
data=$1
a=($(sort -g $data | sed -n '1p;$p' | cut -d' ' -f1))
xmin=$((${a[0]}))
xmax=$((${a[1]}))
n=10 #number of intervals
ymax=$(<$data grep -c '[^[:space:]]')
width=($xmax-$xmin)/$n #interval width
echo "set terminal png size 800,600" > gnuplot_script
echo "set output '$data.png'" >> gnuplot_script
#echo "clear" >> gnuplot_script
#echo "reset" >> gnuplot_script
echo "set key off" >> gnuplot_script
echo "set title $2" >> gnuplot_script
echo "set ylabel 'frequency'" >> gnuplot_script
echo "set xlabel 'byte per second'" >> gnuplot_script
echo "set offset graph 0.05,0.05,0.05,0.0" >> gnuplot_script
echo "set xr [$xmin-$width:$xmax+$width]" >> gnuplot_script
echo "set yr [0:$ymax]" >> gnuplot_script
echo "set xtics $xmin,($xmax-$xmin)/5,$xmax" >> gnuplot_script
echo "set nokey" >> gnuplot_script
echo "set boxwidth $width*0.5" >> gnuplot_script
echo "bin_width = $width">>gnuplot_script
echo "bin_number(x) = floor(x/bin_width)" >> gnuplot_script
echo "rounded(x) = bin_width * ( bin_number(x) + 0.5  )" >> gnuplot_script
echo "set style fill solid border -1" >> gnuplot_script  #fillstyle 
echo "set tics out nomirror">> gnuplot_script
echo "plot '$data'  u (rounded(\$1)):(1) smooth freq w boxes lc rgb\"green\" notitle " >> gnuplot_script
gnuplot gnuplot_script
rm gnuplot_script

