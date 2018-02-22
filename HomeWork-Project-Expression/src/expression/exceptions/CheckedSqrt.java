package expression.exceptions;

import expression.TripleExpression;

/**
 * Created by User on 12.04.2017.
 */
public class CheckedSqrt extends UnaryOperation implements TripleExpression {
    public CheckedSqrt(TripleExpression first) {
        super(first);
    }

    private void check(int a) throws ParseException {
        if (a < 0) {
            throw new ParseException("Negative value of sqrt");
        }
    }
    protected int operation(int a) throws ParseException {
        check(a);
        if (a <= 1) {
            return a;
        }
        int l = 0, r = a;
        while (r - l > 1) {
            int mid = (l + r) / 2;
            if (mid < Integer.MAX_VALUE / mid) {
                if (mid * mid > a) {
                    r = mid;
                } else {
                    l = mid;
                }
            } else {
                r = mid;
            }
        }
        return l;
    }
}
