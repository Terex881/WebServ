<?php
	$n1 = 2;
	$n2 = 4;


	echo "\nMy first PHP script with CGI ! ". ($n1 + $n2). "     \n";

	echo 'param 1 = '.getenv('fname') . "\n";
	echo 'param 2 = '.getenv('lname') . "\n";
	// echo 'param 3 = '.getenv('dd') . "\n";

?>
