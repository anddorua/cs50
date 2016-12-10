/**
 * scripts.js
 *
 * Computer Science 50
 * Problem Set 7
 *
 * Global JavaScript, if any.
 */

/**
 * Number.prototype.format(n, x, s, c)
 * 
 * @param integer n: length of decimal
 * @param integer x: length of whole part
 * @param mixed   s: sections delimiter
 * @param mixed   c: decimal delimiter
 * 
 * see http://stackoverflow.com/questions/149055/how-can-i-format-numbers-as-money-in-javascript/14428340#comment34151293_14428340
 */
Number.prototype.format = function(n, x, s, c) {
    var re = '\\d(?=(\\d{' + (x || 3) + '})+' + (n > 0 ? '\\D' : '$') + ')',
        num = this.toFixed(Math.max(0, ~~n));

    return (c ? num.replace('.', c) : num).replace(new RegExp(re, 'g'), '$&' + (s || ','));
};

function getSymbolCost(symbol) {
    var position = positions.find(function(item) { return item.symbol == symbol; } );
    return position ? Number(position.stock.price) : 0;
}

function getSymbolAmount(symbol) {
    var position = positions.find(function(item) { return item.symbol == symbol; } );
    return position ? Number(position.shares) : 0;
}

function displayActionCost(symbol, cost) {
    $(".cost-gauge[data-symbol=\"" + symbol + "\"]").text(cost.format(2, 3, ',', '.'));
}

function enableSellBtn(symbol, amount) {
    $("[data-action=\"sell\"][data-symbol=\"" + symbol + "\"]").prop('disabled', amount > getSymbolAmount(symbol));
}

function enableBuyBtn(symbol, amount) {
    $("[data-action=\"buy\"][data-symbol=\"" + symbol + "\"]").prop('disabled', 
        amount * getSymbolCost(symbol) > cash
    );
}

function fillFreeItem(stock) {
    var position = positions.find(function(item) { return item.id == ""; } );
    if (stock !== false) {
        position.symbol = stock.symbol;
        position.stock = stock;
    } else {
        position.symbol = "";
        position.stock.symbol = "";
        position.stock.name = "";
        position.stock.price = 0;
    }
}

function displayFreeItem() {
    var position = positions.find(function(item) { return item.id == ""; } );
    $(".price-cell[data-symbol=\"\"]").text(position.stock.price);
    $(".free-amount-control").get()[0].dataset.symbol = position.symbol;
    $(".free-cost-gauge").get()[0].dataset.symbol = position.symbol;
    $(".free-buy-btn").get()[0].dataset.symbol = position.symbol;
    $(".free-hidden").get()[0].value = position.symbol;
}

function initPortfolioController() {
    $("input[name=\"amount\"]").on("change paste keyup", function(msg){
        // calc position change cost
        var amount = Number(this.value);
        if ($.isNumeric(amount)) {
            var cost = amount * getSymbolCost(this.dataset.symbol);
        } else {
            cost = 0;
        }
        displayActionCost(this.dataset.symbol, cost);
        enableSellBtn(this.dataset.symbol, amount);
        enableBuyBtn(this.dataset.symbol, amount);
    });

    var quoteTask = null;

    function clearQuoteTask() {
        if (quoteTask) {
            clearTimeout(quoteTask);
            quoteTask = null;
        }
    }

    $("input[name=\"free-symbol\"]").on("change paste keyup", function(msg){
        clearQuoteTask();
        var symbol = this.value;
        quoteTask = setTimeout(function(){ 
            // request quote
            $.ajax({
                type: "GET",
                url: "/quote.php",
                data: {"symbol": symbol},
                dataType:'json'
            })
            .done(function( data, textStatus, jqXHR ) {
                fillFreeItem(data.stock);
                displayFreeItem();
            })
            .fail(function(err, textStatus){
                console.error(err, textStatus);
            });
        }, 500);
    });

    
}