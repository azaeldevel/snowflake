<?php

if(isset($_GET["number"])) 
{
	$number = $_GET["number"];
	$number++;
	echo $number;
}
else
{
	echo "#";
}

?>