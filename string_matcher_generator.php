<?php
/*
Copyright (C) 2015 Jesse Busman
You may not distribute this program or parts of this
program in any way, shape or form without written
permission from Jesse Busman (born 17 march 1996).
This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. This message may not be changed
or removed.
*/

$strings = file_get_contents($argv[1]);

$noMatchReturnValue = "_TAG_CUSTOM";

$strings = explode("\n", $strings);
$lengths = array();
for ($i=0;$i<count($strings);$i++)
{
	if ($i==0)
	{
		echo $strings[$i]."\n";
		continue;
	}
	if ($i==1)
	{
		$noMatchReturnValue = $strings[$i];
		continue;
	}
	$p = explode("=", $strings[$i]);
	if (count($p)==2)
	{
		if (!isset($lengths[strlen($p[0])])) $lengths[strlen($p[0])] = array();
		$lengths[strlen($p[0])][] = $p;
	}
}
ksort($lengths);

echo "{\n";
echo " int s = 0;\n";
echo " ConstStrIterator i = tag.iterate();\n";

$lengthKeys = array_keys($lengths);
for ($i=0;$i<count($lengthKeys);$i++)
{
	$length = $lengthKeys[$i];
	echo " if (tag.length==$length)\n";
	echo " {\n";
	if ($length!=1)
	{
		echo "  for(;i<tag.length;i++)\n";
		echo "  {\n";
	}
	$states = array();
	$stateCount = 1;
	for ($l=0;$l<$length;$l++) // Loop over char pos
	{
		$states[$l] = array();
		$prevChar = '#';
		for ($j=0;$j<count($lengths[$length]);$j++) // Loop over each string
		{
			$str = $lengths[$length][$j][0];
			if ($str[$l]!=$prevChar)
			{
				$states[$l][$j] = $stateCount++;
			}
			elseif ($l==0 || $states[$l-1][$j]==$states[$l-1][$j-1])
			{
				$states[$l][$j] = $states[$l][$j-1];
			}
			else
			{
				$states[$l][$j] = $stateCount++;
			}
			$prevChar = $str[$l];
		}
	}
	/*echo "<table style='color:red;'>";
	echo "<tr>";
	for ($q=0;$q<count($states[0]);$q++)
	{
		echo "<td>".$lengths[$length][$q][0]."</td>";
	}
	echo "</tr>";
	$p = 0;
	foreach ($states as $state)
	{
		echo "<tr>";
		foreach ($state as $stat)
		{
			echo "<td>$stat</td>";
		}
		echo "</tr>";
		$p++;
	}
	echo "</table>";*/
	if ($length!=1)
	{
		echo "   if (s==0)\n";
		echo "   {\n";
	}
	$prevChar = '#';
	for ($q=0;$q<count($states[0]);$q++)
	{
		$c = $lengths[$length][$q][0][0];
		if ($prevChar!=$c)
		{
			echo "    ";
			if ($q!=0) echo "else ";
			echo "if (*i == '$c') ";
			if ($length==1)
				echo "return ".$lengths[$length][$q][1].";\n";
			else
				echo "s = ".$states[0][$q].";\n";
			$prevChar = $c;
		}
	}
	if ($length!=1)
	{
		echo "    else return $noMatchReturnValue;\n";
		echo "   }\n";
		echo "   else ";
	}
	$prevState = -1;
	for ($p=0;$p<count($states)-1;$p++)
	{
		$prevChar = '#';
		for ($q=0;$q<count($states[$p]);$q++)
		{
			$s = $states[$p][$q];
			if ($s!=$prevState)
			{
				if ($prevState!=-1)
				{
					echo "    else return $noMatchReturnValue;\n";
					echo "   }\n";
				}
				echo "   ";
				if ($prevState!=-1) echo "else ";
				echo "if (s==$s)\n";
				echo "   {\n";
				$prevChar = '#';
			}
			$c = $lengths[$length][$q][0][$p+1];
			if ($prevState!=$s || $prevChar!=$c)
			{
				echo "    ";
				if ($prevChar!='#') echo "else ";
				echo "if (*i == '$c') ";
				if ($length!=1 && $p<count($states)-2)
					echo "s = ".$states[$p+1][$q].";\n";
				else
					echo "return ".$lengths[$length][$q][1].";\n";
			}
			$prevChar = $c;
			$prevState = $s;
		}
	}
	echo "    ";
	if ($length!=1) echo "else ";
	echo "return $noMatchReturnValue;\n";
	if ($i!=0) echo "   }\n";
	if ($length!=1) echo "  }\n";
	echo " }\n";
	flush();
}
echo " return $noMatchReturnValue;\n";
echo "}\n";
?>
