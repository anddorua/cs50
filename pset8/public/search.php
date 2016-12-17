<?php

    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];

    if (isset($_GET['geo']) && strlen($_GET['geo']) >= 2) {
        // CS50 Library
        if (is_numeric($_GET['geo'])) {
            $places = CS50::query("select * from places where postal_code like ? limit 0, 20", $_GET['geo'] . '%');
        } else {
            $mask = implode(' ', array_map(function($word){ return $word . '*'; }, explode(' ', $_GET['geo'])));
            $places = CS50::query(
                "SELECT *, MATCH (`postal_code` ,  `place_name` ,  `admin_name1` ,  `admin_code1` ,  `admin_name2` ,  `admin_code2` ,  `admin_name3` ,  `admin_code3`)"
                . " AGAINST (? in boolean mode) as score FROM places "
                . " WHERE MATCH (`postal_code` ,  `place_name` ,  `admin_name1` ,  `admin_code1` ,  `admin_name2` ,  `admin_code2` ,  "
                . " `admin_name3` ,  `admin_code3`) AGAINST (? IN BOOLEAN MODE) ORDER BY score DESC LIMIT 0 , 20", $mask, $mask);
        }
    }

    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>