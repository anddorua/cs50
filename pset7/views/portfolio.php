<?php
/**
 * Here we modify $positions array to add one free line
 **/
$positions[] = [
    "id" => "",
    "symbol" => "",
    "shares" => 0,
    "stock" => ["symbol" => "","name" => "","price" => 0],
];
?>

<script type="text/javascript">
    $(document).ready(function(){
       $(".popover-symbol").popover();
        initPortfolioController();
    });
</script>
<script type="text/javascript">
    <?php 
        echo "var positions = ";
        echo json_encode($positions) . ';';
        echo "var cash = " . $cash . ';';
    ?>
</script>
<?php if (!empty($error)) : ?>
<div class="row">
    <div class="alert alert-danger alert-dismissible">
        <?=$error?>
    </div>
</div>
<?php endif;?>
<div class="row">
    <form class="form-inline">
        <div class="form-group">
            <label for="cache">Cash:</label>
            <input type="text" class="form-control" id="cache" style="text-align: right;" value="<?=number_format($cash, 4)?>" readonly/>
        </div>
    </form>
</div>
<div class="row">
    <table class="table table-striped">
        <thead>
            <tr>
                <th>Symbol</th>
                <th>Amount</th>
                <th>Price</th>
                <th>Value</th>
                <th>Action</th>
            </tr>
        </thead>
        <tbody>
            <?php foreach($positions as $position): ?>
            <tr>
                <td>
                    <?php if ($position['symbol'] <> ""): ?>
                    <a class="popover-symbol" data-trigger="hover"
                        title="<?=$position['symbol']?>" 
                        data-content="<?=$position['stock']['name']?>"
                        data-placement="top"
                        href="http://finance.yahoo.com/quote/<?=$position['symbol']?>?p=<?=$position['symbol']?>"
                        target="_blank">
                        <?=$position['symbol']?>
                    </a>
                    <?php else: ?>
                    <form class="form-inline">
                        <div class="form-group stock-act-form-el">
                            <input type="text" class="form-control free-symbol" name="free-symbol"/>
                        </div>
                    </form>
                    <?php endif; ?>
                </td>    
                <td align="right"><?=$position['shares']?></td>    
                <td align="right" class="price-cell" data-symbol="<?=$position['symbol']?>"><?=$position['stock'] !== false ? number_format($position['stock']['price'], 4) : 'not found'?></td>    
                <td align="right"><?=$position['stock'] !== false ? number_format($position['stock']['price'] * $position['shares'], 2) : ''?></td>    
                <td align="left">
                    <form class="form-inline symbol-action" method="POST">
                        <input type="hidden" class="<?= $position['symbol'] == "" ? " free-hidden" : ""?>" name="symbol" value="<?=$position['symbol']?>"/>
                        <div class="form-group stock-act-form-el">
                            <button type="submit" class="btn btn-primary" data-action="sell" data-symbol="<?=$position['symbol']?>" name="sell"<?= $position['symbol'] == "" ? " disabled" : ""?>>Sell</button>
                        </div>
                        <div class="form-group stock-act-form-el">
                            <input type="number" class="form-control<?= $position['symbol'] == "" ? " free-amount-control" : ""?>" min="0" data-symbol="<?=$position['symbol']?>" name="amount"/>
                        </div>
                        <div class="form-group stock-act-form-el">
                            <button type="submit" class="btn btn-success<?= $position['symbol'] == "" ? " free-buy-btn" : ""?>"  data-action="buy" data-symbol="<?=$position['symbol']?>" name="buy">Buy</button>
                        </div>
                        <div class="form-group stock-act-form-el">
                            <div class="col-sm-1">
                                  <p class="form-control-static text-warning cost-gauge<?= $position['symbol'] == "" ? " free-cost-gauge" : ""?>" data-symbol="<?=$position['symbol']?>"></p>
                            </div>
                        </div>
                    </form>
                </td>
            </tr>
            <?php endforeach ?>

        </tbody>
        <tfoot>
            <tr>
                <td colspan="3"></td>
                <td align="right">
                    <?php
                    $res = array_reduce($positions, function($sum, $position){ 
                        return $sum += $position['stock'] !== false ? $position['stock']['price'] * $position['shares'] : 0; 
                    }, 0);
                    echo number_format($res, 2);
                    ?>
                </td>
                <td></td>
            </tr>
        </tfoot>
    </table>
</div>
<div class="row">
    
</div>