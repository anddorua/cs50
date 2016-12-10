<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        if (isset($_GET["symbol"])) {
            // ajax request fro quote
            header("Content-Type: application/json");
            echo json_encode([ "stock" => lookup($_GET["symbol"]) ]);
        } else {
            // else render form
            render("stock_insert_form.php", ["title" => "Stock find", "error" => null]);
        }
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST['symbol'])) {
            render("stock_insert_form.php", ["title" => "Stock find", "error" => "provide stock symbol"]);
        } else {
            $stock = lookup($_POST["symbol"]);
            if ($stock === false) {
                render("stock_insert_form.php", ["title" => "Stock find", "error" => "stock symbol not found"]);
            } else {
                render("stock_show.php", ["title" => "Stock info", "stock" => $stock, "error" => null]);
            }
        }
    }

?>