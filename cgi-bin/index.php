<?php
	$n1 = 2;
	$n2 = 4;


	echo "\nMy first PHP script with CGI ! ". ($n1 + $n2). "     \n";

	echo 'param 1 = '.getenv('name') . "\n";
	echo 'param 1 = '.getenv('ff') . "\n";
	print_r ($_GET);
?>
