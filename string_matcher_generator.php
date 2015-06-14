<style>
div
{
	margin-left:20px;
}
</style>
<?php
$strings = "
!doctype=TAG_DOCTYPE
a=TAG_A
abbr=TAG_ABBR
acronym=TAG_ACRONYM
address=TAG_ADDRESS
applet=TAG_APPLET
area=TAG_AREA
article=TAG_ARTICLE
aside=TAG_ASIDE
audio=TAG_AUDIO
b=TAG_B
base=TAG_BASE
basefont=TAG_BASEFONT
bdi=TAG_BDI
bdo=TAG_BDO
big=TAG_BIG
blockquote=TAG_BLOCKQUOTE
body=TAG_BODY
br=TAG_BR
button=TAG_BUTTON
canvas=TAG_CANVAS
caption=TAG_CAPTION
center=TAG_CENTER
cite=TAG_CITE
code=TAG_CODE
col=TAG_COL
colgroup=TAG_COLGROUP
datalist=TAG_DATALIST
dd=TAG_DD
del=TAG_DEL
details=TAG_DETAILS
dfn=TAG_DFN
dialog=TAG_DIALOG
dir=TAG_DIR
div=TAG_DIV
dl=TAG_DL
dt=TAG_DT
em=TAG_EM
embed=TAG_EMBED
fieldset=TAG_FIELDSET
figcaption=TAG_FIGCAPTION
figure=TAG_FIGURE
font=TAG_FONT
footer=TAG_FOOTER
form=TAG_FORM
frame=TAG_FRAME
frameset=TAG_FRAMESET
head=TAG_HEAD
header=TAG_HEADER
h1=TAG_H1
h2=TAG_H2
h3=TAG_H3
h4=TAG_H4
h5=TAG_H5
h6=TAG_H6
hr=TAG_HR
html=TAG_HTML
i=TAG_I
iframe=TAG_IFRAME
img=TAG_IMG
input=TAG_INPUT
ins=TAG_INS
kbd=TAG_KBD
keygen=TAG_KEYGEN
label=TAG_LABEL
legend=TAG_LEGEND
li=TAG_LI
link=TAG_LINK
main=TAG_MAIN
map=TAG_MAP
mark=TAG_MARK
menu=TAG_MENU
menuitem=TAG_MENUITEM
meta=TAG_META
meter=TAG_METER
nav=TAG_NAV
noframes=TAG_NOFRAMES
noscript=TAG_NOSCRIPT
object=TAG_OBJECT
ol=TAG_OL
optgroup=TAG_OPTGROUP
option=TAG_OPTION
output=TAG_OUTPUT
p=TAG_P
param=TAG_PARAM
pre=TAG_PRE
progress=TAG_PROGRESS
q=TAG_Q
rp=TAG_RP
rt=TAG_RT
ruby=TAG_RUBY
s=TAG_S
samp=TAG_SAMP
script=TAG_SCRIPT
section=TAG_SECTION
select=TAG_SELECT
small=TAG_SMALL
source=TAG_SOURCE
span=TAG_SPAN
strike=TAG_STRIKE
strong=TAG_STRONG
style=TAG_STYLE
sub=TAG_SUB
summary=TAG_SUMMARY
sup=TAG_SUP
table=TAG_TABLE
tbody=TAG_TBODY
td=TAG_TD
textarea=TAG_TEXTAREA
tfoot=TAG_TFOOT
th=TAG_TH
thead=TAG_THEAD
time=TAG_TIME
title=TAG_TITLE
tr=TAG_TR
track=TAG_TRACK
tt=TAG_TT
u=TAG_U
ul=TAG_UL
var=TAG_VAR
video=TAG_VIDEO
wbr=TAG_WBR
";

echo "HTMLTagType getTagType(ConstStr tag)<br/>";
echo "{<br/><div>";
echo "int s = 0;<br/>";
echo "ConstStrIterator i = tag.iterate();<br/>";

$strings = explode("\n", $strings);
$lengths = array();
for ($i=0;$i<count($strings);$i++)
{
	$p = explode("=", $strings[$i]);
	if (count($p)==2)
	{
		if (!isset($lengths[strlen($p[0])])) $lengths[strlen($p[0])] = array();
		$lengths[strlen($p[0])][] = $p;
	}
}
ksort($lengths);


$lengthKeys = array_keys($lengths);
for ($i=0;$i<count($lengthKeys);$i++)
{
	$length = $lengthKeys[$i];
	echo "if (tag.length==$length)<br/>";
	echo "{<br/><div>";
	if ($length!=1)
	{
		echo "for(;i&lt;tag.length;i++)<br/>";
		echo "{<br/><div>";
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
		echo "if (s==0)<br/>";
		echo "{<br/><div>";
	}
	$prevChar = '#';
	for ($q=0;$q<count($states[0]);$q++)
	{
		$c = $lengths[$length][$q][0][0];
		if ($prevChar!=$c)
		{
			if ($q!=0) echo "else ";
			echo "if (*i == '$c') ";
			if ($length==1)
				echo "return ".$lengths[$length][$q][1].";<br/>";
			else
				echo "s = ".$states[0][$q].";<br/>";
			$prevChar = $c;
		}
	}
	if ($length!=1)
	{
		echo "else return _TAG_CUSTOM;<br/>";
		echo "</div>}<br/>else ";
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
					echo "else return _TAG_CUSTOM;";
					echo "</div>}<br/>else ";
				}
				echo "if (s==$s)<br/>{<br/><div>";
				$prevChar = '#';
			}
			$c = $lengths[$length][$q][0][$p+1];
			if ($prevState!=$s || $prevChar!=$c)
			{
				if ($prevChar!='#') echo "else ";
				echo "if (*i == '$c') ";
				if ($length!=1 && $p<count($states)-2)
					echo "s = ".$states[$p+1][$q].";<br/>";
				else
					echo "return ".$lengths[$length][$q][1].";<br/>";
			}
			$prevChar = $c;
			$prevState = $s;
		}
	}
	if ($length!=1) echo "else ";
	echo "return _TAG_CUSTOM;";
	if ($i!=0) echo "</div>}<br/>";
	if ($length!=1) echo "</div>}<br/>";
	echo "</div>}<br/>";
	flush();
}
echo "return _TAG_CUSTOM;";
echo "</div>}<br/>";
?>
