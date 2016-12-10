<form action="deposit.php" method="post">
    <fieldset>
        <div class="form-group">
            <input autocomplete="off" autofocus class="form-control" name="amount" placeholder="Deposit amount" type="text"/>
            <?php if (!empty($error)): ?>
            <span class="help-block text-danger"><?= $error ?></span>
            <?php endif; ?>
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                <span aria-hidden="true" class="glyphicon glyphicon-usd"></span>
                Add
            </button>
        </div>
    </fieldset>
</form>
