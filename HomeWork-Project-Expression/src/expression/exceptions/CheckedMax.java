package expression.exceptions;

import expression.BinaryOperator;
import expression.TripleExpression;

/**
 * Created by User on 11.04.2017.
 */
public class CheckedMax extends BinaryOperator implements TripleExpression {
    public CheckedMax(TripleExpression first, TripleExpression second) {
        super(first, second);
    }

    protected int operation(int a, int b) throws ParseException {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }
}