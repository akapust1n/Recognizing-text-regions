# ------------- SCRIPT ------------- #

#!/bin/bash
echo "# arguments called with ---->  ${@}     "
echo "# \$1 number of file ---------------------->  $1       "
echo "# \$2 destination dir  ---------------------->  $2       "

find . -maxdepth 1 -type f |head -$1|xargs cp -t $2
echo "Moved!"
exit
