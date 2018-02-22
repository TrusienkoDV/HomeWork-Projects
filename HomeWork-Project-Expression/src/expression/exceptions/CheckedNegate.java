package expression.exceptions;

import expression.TripleExpression;

/**
 * Created by User on 12.04.2017.
 */
public class CheckedNegate extends UnaryOperation implements TripleExpression {
    public CheckedNegate(TripleExpression first) {
        super(first);
    }

    protected int operation(int a) throws ParseException, OverflowException {
        check(a);
        return -a;
    }

    private void check(int a) throws ParseException, OverflowException {
        if (a == Integer.MIN_VALUE) {
            throw new OverflowException();
        }
    }
}
