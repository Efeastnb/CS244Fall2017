<?php
 
  if( $_REQUEST["Heartrate"] ||  $_REQUEST["Bloodpressure"]  ) 
   {
   echo " The Heartrate is: ". $_REQUEST['Heartrate']. "%<br />";
   echo " The Bloodpressure is: ". $_REQUEST['Bloodpressure']. " Celcius<br />";
  
   }
	  	
    $var1 = $_REQUEST['Heartrate'];
    $var2 = $_REQUEST['Bloodpressure'];


    $WriteMyRequest=
    "<p> The Heartrate is : "       . $var1 . " </p>".
    "<p> The Bloodpressure is : " . $var2 . "</p><br/>";


    file_put_contents('messagefromesp.html',$WriteMyRequest , FILE_APPEND);

?>