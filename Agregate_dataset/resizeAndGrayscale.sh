# ------------- SCRIPT ------------- #

#!/bin/bash

echo
echo "# arguments called with ---->  ${@}     "
echo "# \$1 file extension ---------------------->  $1       "
echo "# \$2 square size  ---------------------->  $2       "

counter=0
rm -rf result
#Возможно, стоит добавить какие-нибудь опции для imagemagick
mkdir result
for img in `ls *."$1"`
do
  convert $img  -type Grayscale -resize $2x$2\! result/processed_$img
  counter=$(($counter+1))
done
echo "# Argegated $counter images"
exit
