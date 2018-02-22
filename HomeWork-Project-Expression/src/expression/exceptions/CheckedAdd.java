package expression.exceptions;

import expression.BinaryOperator;
import expression.TripleExpression;


public class CheckedAdd extends BinaryOperator implements TripleExpression {
    public CheckedAdd(TripleExpression first, TripleExpression second) {
        super(first, second);
    }

    private void check(int a, int b) throws ParseException, OverflowException {
        if (a > 0 && b > Integer.MAX_VALUE - a) {
            throw new OverflowException();
        }
        if (a < 0 && b < Integer.MIN_VALUE - a) {
            throw new OverflowException();
        }
    }

    protected int operation(int a, int b) throws ParseException, OverflowException {
        check(a, b);
        return a + b;
    }
}
