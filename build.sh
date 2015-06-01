# Copyright (C) 2015 Jesse Busman
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>


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
	if [[ $i != test* ]] # && [[ $i != css* ]] && [[ $i != html_yacc* ]];
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
		g++ $dotOfiles -std=c++11 -lX11 -lpthread -o $outputProgramFile
		echo "Done."
	else
		echo "No changes detected. Nothing was recompiled."
	fi
else
	echo "Linking..."
	g++ $dotOfiles -std=c++11 -lX11 -lpthread -o $outputProgramFile
	echo "Done."
fi
