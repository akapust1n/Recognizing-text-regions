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
  if ($1 == "text") 
  then
  convert $img  -type Grayscale -resize $2x$2\! result/text_processed_$img
  else
  convert $img  -type Grayscale -resize $2x$2\! result/notext_processed_$img
  fi
  counter=$(($counter+1))
done
echo "# Argegated $counter images"
exit
