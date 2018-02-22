package expression.exceptions;

import expression.BinaryOperator;
import expression.TripleExpression;

/**
 * Created by User on 12.04.2017.
 */
public class CheckedMultiply extends BinaryOperator implements TripleExpression {
    public CheckedMultiply(TripleExpression first, TripleExpression second) {
        super(first, second);
    }

    private void check(int a, int b) throws ParseException, OverflowException {
        int t;
        if (a > b) {
            t = a;
            a = b;
            b = t;
        } 
        if (a < 0) {
            if (b < 0) {
                if (a < Integer.MAX_VALUE / b) {
                    throw new OverflowException();
                }
            } else if (b > 0) {
                if (Integer.MIN_VALUE / b > a) {
                    throw new OverflowException();
                }
            }
        } else if (a > 0) {
            if (a > Integer.MAX_VALUE / b) {
                throw new OverflowException();
            }
        }
    }

    protected int operation(int a, int b) throws ParseException, OverflowException {
        check(a, b);
        return a * b;
    }
}
