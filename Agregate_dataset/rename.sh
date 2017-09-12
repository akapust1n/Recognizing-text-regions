# ------------- SCRIPT ------------- #

#!/bin/bash

counter=0
for img in `ls *.jpg`
do
  mv $img  processed_$counter.jpg
  counter=$(($counter+1))
done
for img in `ls *.JPG`
do
  mv $img  processed_$counter.jpg
  counter=$(($counter+1))
done
echo "# Argegated $counter images"
exit
