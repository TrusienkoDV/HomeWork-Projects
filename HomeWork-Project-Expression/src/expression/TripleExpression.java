package expression;

import expression.exceptions.OverflowException;
import expression.exceptions.ParseException;

public interface TripleExpression {
    int evaluate(int x, int y, int z) throws ParseException, OverflowException;
}
