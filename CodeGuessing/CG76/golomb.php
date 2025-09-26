<?php
$table = array();
$table[1] = 1;

function entry($x) {
    global $table;
    if(array_key_exists($x, $table)) {
        return $table[$x];
    }
    
    $table[$x] = 1 + entry($x - entry(entry($x - 1)));
    return $table[$x];
}

?>
