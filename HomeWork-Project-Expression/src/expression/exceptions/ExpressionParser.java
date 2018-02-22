package expression.exceptions; 

import expression.*;

public class ExpressionParser implements Parser{ 
	private String exp, var, token; 
	private int n = 0, number; 
	
	private final static String FA = "The first argument of the function ",
						 SA = "The second argument of the function ",
						 A = "Function argument ";

	private boolean isMinusGood() throws ParseException, OverflowException {
		while(n < exp.length() && Character.isWhitespace(exp.charAt(n))) { 
			n++; 
		} 
		if (n >= exp.length()) {
			return true;
		}
			
		if (Character.isDigit(exp.charAt(n))) { 
			readNumber(token); 
			return false; 
		} else { 
			return true; 
		} 
	} 
	
	private void check(TripleExpression fl, String message, String operator, int pos) throws ParseException {
        if (fl == null) {
			throw new ParseException(message + operator + " in position " + pos + " not found");
		}
    }
 
	private void readNumber(String ch) throws ParseException, OverflowException {
		String newNumber = "";
		newNumber = newNumber + ch; 
		int s = n, l = 0;
		
		while (n < exp.length() && Character.isDigit(exp.charAt(n))){
			n++;
			l++;
		} 
		
		newNumber = updateString(newNumber, s, l);
		
		try {
			number = Integer.parseInt(newNumber); 
		} catch (NumberFormatException e) {
                throw new OverflowException("Too large number in position " + n);
            }
		
		token = "N"; 
	} 


	private String sumString(String a, String b) { 
		return a + b; 
	} 

	private String updateString(String w, int s, int l) {
		if (l != 0){
			return  sumString(w, exp.substring(s, s+l));
		}
		return w;
	}

	private String readWord() throws ParseException { 
		String newWord = "";
		int s = n, l = 0;
		while (n < exp.length() && (Character.isLetter(exp.charAt(n)) || Character.isDigit(exp.charAt(n))) ) { 
			n++;
			l++;
		} 
		
		newWord = updateString(newWord, s, l);
		
		return newWord; 
	} 

	private void getToken() throws ParseException, OverflowException {
		String newToken;
		while (n >= exp.length() || Character.isWhitespace(exp.charAt(n)) == true) { 
			if (n >= exp.length()) { 
				token = "E";
				return ; 
			} 
			n++; 
		} 
		
		if (Character.isLetter(exp.charAt(n))) { 
			newToken = readWord(); 
		} else { 
			newToken = Character.toString(exp.charAt(n));
			n++;
		} 
	
		
		switch (newToken) { 
			case "x": 
			case "y": 
			case "z": { 
				token = "V"; 
				var = newToken; 
				return; 
			} 
			case "abs": 
			case "sqrt":
			case "-": 
			case "+": 
			case "*": 
			case "max":
			case "min":
			case "/": 
			case "(": 
			case ")": 
			{ 
				token = newToken; 
				return; 
			} 
			case "1": case "2": case "3": case "4": 
			case "5": case "6": case "7": case "8": 
			case "9": case "0": 
			{ 
				readNumber(newToken); 
				return; 
			}
			default:
                    throw new ParseException("Uncertain expression '"+ newToken + "' in position " + n);
		} 
	} 

	private TripleExpression prim() throws ParseException, OverflowException {
		getToken(); 
		switch (token) { 
			case "N": { 
				getToken(); 
				return (new Const(number)); 
			} 
			case "V": { 
				getToken(); 
				return (new Variable(var)); 
			} 
			case "-": { 
				if(isMinusGood()) { 
					return new CheckedNegate(readU("-", n)); 
				} else { 
					getToken(); 
					return new Const(number); 
				} 
			} 
			case "(": { 
				int k = n;
				TripleExpression e = minmax();
				if (!token.equals(")")) {
					throw new ParseException("There is no closing bracket for the bracket in position " + k);
				}
				
				getToken(); 
				return e; 
			} 
			case "abs": { 
				return new CheckedAbs(readU("abs", n)); 
			}  
			case "sqrt": {
				return new CheckedSqrt(readU("sqrt", n)); 
			}

			default: 
				return null; 
		} 
	} 
	
	private TripleExpression readU(String op, int pos) throws ParseException, OverflowException {
		TripleExpression arg = prim();
		check(arg, A, op, pos);
		return arg;
	}

	private TripleExpression term() throws ParseException, OverflowException {
		TripleExpression left = prim(); 
		while (true) { 
			switch (token) { 
				case "*": { 
					left = new CheckedMultiply(left, readSecond(left, "'*'", 2, n)); 
					break; 
				} 
				case "/": { 
					left = new CheckedDivide(left, readSecond(left, "'/'", 2, n)); 
					break; 
				} 
				default : {
					return left;
				} 
			} 
		} 
	} 

	private TripleExpression readSecond(TripleExpression left, String op, int lvl, int pos) throws ParseException, OverflowException {
		check(left, FA, op, pos);
		TripleExpression right;
		
		switch (lvl) {
			case 0: {
				right = expr();
				break;
			}
			case 1: {
				right = term();
				break;
			}
			case 2: {
				right = prim();
				break;
			}
			default: {	
				right = null;
			}
		}
		
		check(right, SA, op, pos);
		return right;
	}

	private TripleExpression expr() throws ParseException, OverflowException {
		TripleExpression left = term(); 
		while (true) { 
		switch (token) { 
			case "+": { 
				left = new CheckedAdd(left, readSecond(left, "'+'", 1, n)); 
				break; 
			} 
			case "-": { 
				left = new CheckedSubtract(left, readSecond(left, "'-'", 1, n)); 
				break; 
			} 
			default : {
				return left;
			} 
			} 
		} 
	} 
	
	private TripleExpression minmax() throws ParseException, OverflowException {
		TripleExpression left = expr(); 
		while (true) { 
		switch (token) { 
			case "max": { 
				left = new CheckedMax(left, readSecond(left, "'max'", 0, n)); 
				break; 
			} 
			case "min": { 
				left = new CheckedMin(left, readSecond(left, "'min'", 0, n)); 
				break; 
			} 
			default : 
				return left; 
			} 
		} 
	} 
	
	private void lol222()
	{
	}

	public TripleExpression parse(String s) throws ParseException, OverflowException {
		exp = s; 
		n = 0;
		TripleExpression temp = minmax(); 
			
		if (token.equals(")")) {
			throw new ParseException("There is no opening bracket for the position in the position " + n);
		}
		if (!token.equals("E")) {
			throw new ParseException("Possible missing function in position " + (n-token.length()) + ". Expression cannot be parsed");
		}
		return temp; 
	} 
}
