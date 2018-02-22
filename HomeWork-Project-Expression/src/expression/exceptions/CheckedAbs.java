package expression.exceptions;

import expression.TripleExpression;

public class CheckedAbs extends UnaryOperation implements TripleExpression{
    public CheckedAbs(TripleExpression first) {
        super(first);
    }

    private void check(int a) throws ParseException, OverflowException {
        if (a == Integer.MIN_VALUE) {
            throw new OverflowException();
        }
    }
    private int abs(int a) throws ParseException {
        if (a < 0) {
            return -a;
        } else {
            return a;
        }
    }

    protected int operation(int a) throws ParseException, OverflowException {
        check(a);
        return abs(a);
    }
}
