package expression;

import expression.exceptions.*;

abstract public class BinaryOperator implements TripleExpression {
    private TripleExpression firstOp;
    private TripleExpression secondOp;

    protected BinaryOperator(TripleExpression first, TripleExpression second) {
        firstOp = first;
        secondOp = second;
    }

    public int evaluate(int x, int y, int z) throws ParseException, OverflowException {
        return operation(firstOp.evaluate(x, y, z), secondOp.evaluate(x, y, z));
    }

    abstract protected int operation(int a, int b) throws ParseException, OverflowException;
}

