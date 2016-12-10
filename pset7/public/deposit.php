<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("deposit_form.php", ["title" => "Deposit funds", "error" => null]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST['amount'])) {
            render("deposit_form.php", ["title" => "Deposit funds", "error" => "provide amount of dollars to add to deposit"]);
        } else {
            require("../models/Portfolio.php"); 
            $portfolio = new Portfolio($_SESSION["id"]);
            if ($portfolio->addDeposit($_POST["amount"])) {
                redirect("/");
            } else {
                render("deposit_form.php", ["title" => "Deposit funds", "error" => $portfolio->getError()]);
            }
        }
    }

?>