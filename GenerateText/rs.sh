#!/bin/bash

echo
echo "# arguments called with ---->  ${@}     "
echo "# \$1 file extension ---------------------->  $1       "
echo "# \$2 square size  ---------------------->  $2       "

counter=0
rm -rf result
#Возможно, стоит добавить какие-нибудь опции для imagemagick
mkdir result
for img in `find . -regex ".*\.\(jpg\|gif\|png\|jpeg\)"`
do
  convert $img  -type Grayscale -resize $2x$2\! result/processed_$counter.$1
  mv result/processed_$counter.$1 $img
  counter=$(($counter+1))
  echo $counter
done
rm -r result
echo "# Argegated $counter images"
exit
