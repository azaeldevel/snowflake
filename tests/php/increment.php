<?php

if(isset($_POST["number"])) 
{
	$number = $_POST["number"];
	$number++;
	echo $number;
}
else
{
	echo "#"
}

?>