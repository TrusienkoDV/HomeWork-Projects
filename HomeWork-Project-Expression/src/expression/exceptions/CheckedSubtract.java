package expression.exceptions;

import expression.BinaryOperator;
import expression.TripleExpression;

/**
 * Created by User on 12.04.2017.
 */
public class CheckedSubtract extends BinaryOperator implements TripleExpression {
    public CheckedSubtract(TripleExpression first, TripleExpression second) {
        super(first, second);
    }

    private void check(int a, int b) throws ParseException, OverflowException {
        if (b > 0 && a < Integer.MIN_VALUE + b) {
            throw new OverflowException();
        }
        if (b < 0 && a > Integer.MAX_VALUE + b) {
            throw new OverflowException();
        }
    }

    protected int operation(int a, int b) throws ParseException, OverflowException {
        check(a, b);
        return a - b;
    }
}
