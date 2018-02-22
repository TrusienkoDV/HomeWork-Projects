package expression.exceptions;

import expression.BinaryOperator;
import expression.TripleExpression;

/**
 * Created by User on 12.04.2017.
 */
public class CheckedDivide extends BinaryOperator implements TripleExpression {
    public CheckedDivide(TripleExpression first, TripleExpression second) {
        super(first, second);
    }

    private void check(int a, int b) throws ParseException, OverflowException {
        if (b == 0) {
            throw new ParseException("Division by zero");
        }
        if (a == Integer.MIN_VALUE && b == -1) {
            throw new OverflowException();
        }
    }

    protected int operation(int a, int b) throws ParseException, OverflowException {
        check(a, b);
        return a / b;
    }
}
