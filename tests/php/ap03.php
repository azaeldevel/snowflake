<?php

if(isset($_GET["number"])) 
{
	$number = $_GET["number"];
	$result = $number * $number;
	$result = $result + sqrt($number);
	echo $result;
}
else
{
	echo "#";
}

?>