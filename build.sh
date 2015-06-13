# Copyright (C) 2015 Jesse Busman
# You may not distribute this program or parts of this
# program in any way, shape or form without written
# permission from Jesse Busman (born 17 march 1996).
# This program is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. This message may not be changed
# or removed.


outputProgramFile="whatr.exe"
dotCpp=".cpp"	# The files you want to compile
dotO=".o"		# The extension compiled files get




#---------Don't change below this line if you don't know what you're doing :)
forceCompileAll=0
if [ $# -eq 0 ]
then
	forceCompileAll=0
else
	forceCompileAll=1
fi
dotOfiles=""
compiled=0
for i in *.cpp; do
	if [[ $i != test* ]] && [[ $i != _gen* ]];
	then
		j="${i/$dotCpp/$dotO}"
		dotOfiles=$dotOfiles$j" "
		if [ -f $j ];
		then
			if (( `date -r $i +%s` > `date -r $j +%s` )) || (( forceCompileAll == 1 ))
			then
				echo "$j has expired. Recompiling $i...";
				compiled=1
				g++ -std=c++11 -c $i
			fi
		else
			echo "Compiling $i..."
			compiled=1
			g++ -std=c++11 -c $i
		fi
	fi
done
if [ -f $outputProgramFile ]
then
	if (( compiled == 1 ))
	then
		echo "Relinking..." 
		g++ $dotOfiles -std=c++11 -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -o $outputProgramFile
		echo "Done."
	else
		echo "No changes detected. Nothing was recompiled."
	fi
else
	echo "Linking..."
	g++ $dotOfiles -std=c++11 -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -o $outputProgramFile
	echo "Done."
fi
