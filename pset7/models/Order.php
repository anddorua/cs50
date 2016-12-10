<?php

class Order
{
    public $symbol;
    public $amount;
    private $error;
    
    public function __construct($symbol, $amount)
    {
        $this->symbol = $symbol;
        $this->amount = $amount;
        $this->error = null;
    }
    
    public function getError()
    {
        return $this->error;
    }
    
    public function validateFields()
    {
        if (empty($this->symbol)) {
            $this->error = "Symbol is empty.";        
            return false;
        }
        if (empty($this->amount)) {
            $this->error = "Amount is empty.";         
            return false;
        }
        if (!is_numeric($this->amount)) {
            $this->error = "Amount should be a numeric value.";           
            return false;
        }
        if ($this->amount < 0) {
            $this->error = "Amount should be a positive number.";            
            return false;
        }
        if (((float)$this->amount - floor($this->amount)) > 0) {
            $this->error = "Amount should be an integer number.";            
            return false;
        };
        return true;
    }
}