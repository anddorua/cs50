<?php

class Portfolio
{
    private $userId;
    private $error = null;
    
    public function __construct($userId)
    {
        $this->userId = $userId;
    }
    
    public function getError()
    {
        return $this->error;
    }
    
    public function getPositions()
    {
        $portfolio = CS50::query("SELECT id, symbol, shares FROM portfolios WHERE user_id = ?", $this->userId);
        $result = array_map(function($row){
            $row['stock'] = lookup($row['symbol']);
            return $row;
        }, $portfolio);
        return $result;
    }
    
    public function getPosition($symbol)
    {
        $portfolio = CS50::query("SELECT id, symbol, shares FROM portfolios WHERE user_id = ? and symbol = ?", $this->userId, $symbol);
        if ($portfolio) {
            return $portfolio[0];
        } else {
            return false;
        }
    }
    
    public function getCash()
    {
        $rows = CS50::query("SELECT cash FROM users WHERE id = ?", $this->userId);
        if ($rows) {
            return $rows[0]['cash'];
        } else {
            return 0;
        }
    }
    
    public function getStock($symbol)
    {
         return lookup($symbol);
    }
    
    public function sell(Order $order)
    {
        $position = $this->getPosition($order->symbol);
        $stock = $this->getStock($order->symbol);
        if (!$this->validateSell($order, $position, $stock)) {
            return false;
        }
        $earn = $order->amount * $stock['price'];
        $rest_amount = $position['shares'] - $order->amount;
        if ($rest_amount == 0) {
            CS50::query("DELETE FROM portfolios WHERE user_id = ? AND symbol = ?", $this->userId, $order->symbol);
        } else {
            CS50::query("update portfolios set shares=shares-? WHERE user_id = ? AND symbol = ?", $order->amount, $this->userId, $order->symbol);
        }
        CS50::query("update users set cash=cash+? WHERE id = ?", $earn, $this->userId);
        $this->addHistory("sell", $order->symbol, $order->amount, $stock['price']);
        return true;
    }
    
    private function validateSell(Order $order, $position, $stock)
    {
        if ($position === false) {
            $this->error = "Symbol not in the case.";
            return false;
        }
        if ((int)$position['shares'] < (int)$order->amount) {
            $this->error = "Amount exceeds available shares.";
            return false;
        }
        if ($stock === false) {
            $this->error = "Symbol cannot be found on finance.yahoo.com.";
            return false;
        }
        return true;
    }
    
    public function buy(Order $order)
    {
        $stock = $this->getStock($order->symbol);
        $cash = $this->getCash();
        if (!$this->validateBuy($order, $cash, $stock)) {
            return false;
        }
        $position = $this->getPosition($order->symbol);
        $spend = $order->amount * $stock['price'];
        if ($position) {
            CS50::query("update portfolios set shares=shares+? WHERE user_id = ? AND symbol = ?", $order->amount, $this->userId, $order->symbol);
        } else {
            CS50::query("insert into portfolios (user_id, symbol, shares) values (?, ?, ?)", $this->userId, $order->symbol, $order->amount);
        }
        CS50::query("update users set cash=cash-? WHERE id = ?", $spend, $this->userId);
        $this->addHistory("buy", $order->symbol, $order->amount, $stock['price']);
        return true;
    }
    
    public function validateBuy(Order $order, $cash, $stock)
    {
        if ($stock === false) {
            $this->error = "Symbol cannot be found on finance.yahoo.com.";
            return false;
        }
        if ($cash < $order->amount * $stock['price']) {
            $this->error = "Cash is not enough.";
            return false;
        }
        return true;
    }
    
    private function addHistory($operation, $symbol, $amount, $price)
    {
        CS50::query("insert into history (user_id, operation, symbol, amount, price) values (?, ?, ?, ?, ?)"
            , $this->userId
            , $operation
            , $symbol
            , $amount
            , $price);
    }
    
    public function getHistory()
    {
        return CS50::query("SELECT * FROM `history` WHERE user_id=?", $this->userId);
    }
    
    public function addDeposit($amount)
    {
        if (!is_numeric($amount)) {
            $this->error = "Amount should be a numeric value.";           
            return false;
        }
        if ($amount < 0) {
            $this->error = "Amount should be a positive number.";            
            return false;
        }
        CS50::query("update users set cash=cash+? where id=?", $amount, $this->userId);
        return true;
    }
}