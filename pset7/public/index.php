<?php

    // configuration
    require("../includes/config.php"); 
    require("../models/Portfolio.php"); 

    $error = null;
    
    $portfolio = new Portfolio($_SESSION["id"]);
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        require("../models/Order.php"); 
        $order = new Order(
            isset($_POST['symbol']) ? $_POST['symbol'] : null,
            isset($_POST['amount']) ? $_POST['amount'] : null
        );
        if (!$order->validateFields()) {
            $error = $order->getError();
        } else {
            if (isset($_POST['buy'])) {
                if (!$portfolio->buy($order)) {
                    $error = $portfolio->getError();
                }
            } else if (isset($_POST['sell'])) {
                if (!$portfolio->sell($order)) {
                    $error = $portfolio->getError();
                }
            } else {
                $error = "unknown action requested";
            }
        }
    }

    // render portfolio
    render("portfolio.php", [
        "title" => "Portfolio", 
        "positions" => $portfolio->getPositions(),
        "cash" => $portfolio->getCash(),
        "error" => $error,
    ]);

?>
