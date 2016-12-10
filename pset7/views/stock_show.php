<form action="quote.php" method="post">
    <fieldset>
         <div class="form-group">
            <label for="stock_symbol">Symbol</label>
            <input type="text" class="form-control" id="stock_symbol" value="<?=$stock['symbol']?>" readonly>
         </div>
        <div class="form-group">
            <label for="stock_name">Name</label>
            <input type="text" class="form-control" id="stock_name" value="<?=$stock['name']?>" readonly>
        </div>
        <div class="form-group">
            <label for="stock_price">Price</label>
            <input type="text" class="form-control" id="stock_price" value="<?=$stock['price']?>" readonly>
        </div>
    </fieldset>
</form>
<div>
    <a href="quote.php">find more</a>
</div>