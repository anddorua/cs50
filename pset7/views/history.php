<div class="row">
    <table class="table table-striped">
        <thead>
            <tr>
                <th>Time</th>
                <th>Transaction</th>
                <th>Symbol</th>
                <th>Amount</th>
                <th>Price</th>
                <th>Cost</th>
            </tr>
        </thead>
        <tbody>
            <?php foreach($history as $hItem): ?>
            <tr>
                <td>
                    <?= $hItem['op_time'] ?>
                </td>
                <td>
                    <?= $hItem['operation'] ?>
                </td>
                <td>
                    <?= $hItem['symbol'] ?>
                </td>
                <td>
                    <?= $hItem['amount'] ?>
                </td>
                <td>
                    <?= number_format($hItem['price'], 2) ?>
                </td>
                <td>
                    <?= number_format($hItem['price'] * $hItem['amount'], 2) ?>
                </td>
            </tr>
            <?php endforeach ?>

        </tbody>
    </table>
</div>
<div class="row">
    
</div>