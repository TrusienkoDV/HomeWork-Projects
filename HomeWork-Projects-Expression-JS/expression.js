var variables = {"x":0, "y":1, "z":2, "u":3, "v":4, "w":5};
var actions = {"+":2, "-":2, "*":2, "/":2, "negate":1, "max5":5, "min3":3}
var consts = {"pi":0, "e":0};
x = variable('x'), y = variable('y'), z = variable('z');

var pi = cnst(Math.PI), e = cnst(Math.E);

function cnst(a) {
    return function() {
        return a;
    }
}

function variable(a) {
    return function() {
        return arguments[variables[a]];
    }
}

function addOperator(a, b) {
    return a + b;
}

function subOperator(a, b) {
    return a - b;
}

function mulOperator(a, b) {
    return a * b;
}

function divOperator(a, b) {
    return a / b;
}

function minOperator() {
    return Math.min.apply(null, arguments);
}

function maxOperator() {
    return Math.max.apply(null, arguments);
}

function negOperator(a) {
    return -a;
}

function calcMore() {
    var upArguments = arguments, moreOperator = this;
    return function () {
        var a = [], i;
        for (i = 0; i < upArguments.length; i++) {
            a[i] = upArguments[i].apply(null, arguments);
        }
        return moreOperator.apply(null, a);
    }
}

function add(a, b) {
    return calcMore.apply(addOperator, arguments);
}

function subtract(a, b) {
    return calcMore.apply(subOperator, arguments);
}

function multiply(a, b) {
    return calcMore.apply(mulOperator, arguments);
}

function divide(a, b) {
    return calcMore.apply(divOperator, arguments);
}

function negate(a) {
    return calcMore.apply(negOperator, arguments);
}

function min3() {
    return calcMore.apply(minOperator, arguments);
}

function max5() {
    return calcMore.apply(maxOperator, arguments);
}

var funcs = {"+":add, "-":subtract, "*":multiply, "/":divide, "negate":negate, "max5":max5, "min3":min3};


function readWord(s, i) {
    var l = i;
    while (s.charAt(i) !== ' ') {
        i++;
    }
    return s.substr(l, i - l);
}

function getCnst(s) {
    switch (s) {
        case "pi": {
            return pi;
        }
        case "e": {
            return e;
        }
    }
}



function parse(s) {
    var i, j, a = [], t;
    j = 0;
	s = s + " End ";

	function giveArg(n) { 

		var b = [];
		var i, k = 0;
		for(i = n-1; i >= 0; i--) { 
			b[k++] = a[j-i-1]; 
		} 
		return b; 
	}

    for (i = 0; i < s.length; i++) {
        while(s.charAt(i) === ' ') {
            i++;
        }
        t = readWord(s, i);
        i += t.length;
        
        if (variables[t] !== undefined) {
			a[j] = variable(t);
            j++;
		} else if (actions[t] !== undefined) {
			a[j - actions[t]] = funcs[t].apply(null, giveArg(actions[t]));
			j -= actions[t] - 1;
		} else if (consts[t] !== undefined) {
			a[j] = getCnst(t);
            j++;
		} else {
        
        switch (t) {
            case "End": {
                return a[0];
            }
            default : {
                a[j] = cnst(parseInt(t));
                j++;
            }
			}
		}
    }
    
    
}

