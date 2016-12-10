<?php

    // configuration
    require("../includes/config.php"); 
    require("../models/Portfolio.php"); 

    $error = null;
    
    $portfolio = new Portfolio($_SESSION["id"]);

    // render portfolio
    render("history.php", [
        "title" => "History", 
        "history" => $portfolio->getHistory(),
    ]);

?>