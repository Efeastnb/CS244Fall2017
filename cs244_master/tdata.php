<?php
    
    if( $_REQUEST["_IR0"] || $_REQUEST["_RED0"] )
    {
        echo " The IR is: ". $_REQUEST['_IR0']. "%<br />";
        echo " The RED is:". $_REQUEST['_RED0']. "%<br />";
    }
    
    $_IR0  = $_REQUEST['_IR0'];
    $_RED0 = $_REQUEST['_RED0'];
    
    //store into csv file
    $list = array($_IR0,$_RED0);
    $file = fopen("ppg6.4.csv","a");
    fputcsv($file,$list);
    fclose($file);
    
    //show data in html format
    $WriteMyRequest=
    "<p>The IR is : "  . $_IR0 . "  </p>".
    "<p>The RED is : "  . $_RED0 . " </p><br/>";

    file_put_contents('ppg.html', $WriteMyRequest, FILE_APPEND);
?>
