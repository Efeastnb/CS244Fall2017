<?php

if( $_REQUEST["_X0"] || $_REQUEST["_Y0"] || $_REQUEST["_Z0"] )
{
    echo " The X is: ". $_REQUEST['_X0']. "%<br />";
    echo " The Y is: ". $_REQUEST['_Y0']. "%<br />";
    echo " The Z is: ". $_REQUEST['_Z0']. "%<br />";
}

$_X0  = $_REQUEST['_X0'];
$_Y0  = $_REQUEST['_Y0'];
$_Z0  = $_REQUEST['_Z0'];


//store into csv file
$list = array($_X0,$_Y0,$_Z0);
$file = fopen("XYZ.csv","a");
fputcsv($file,$list);
fclose($file);

//show data in html format
$WriteMyRequest=
"<p>The X is : "  . $_X0 . "  </p>".
"<p>The Y is : "  . $_Y0 . "  </p>".
"<p>The Z is : "  . $_Z0 . "  </p><br/>";
file_put_contents('XYZ.html', $WriteMyRequest, FILE_APPEND);


?>
