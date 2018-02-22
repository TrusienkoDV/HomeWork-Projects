package expression.exceptions;

import expression.TripleExpression;


abstract public class UnaryOperation implements TripleExpression{
    private TripleExpression firstOp;

    protected UnaryOperation(TripleExpression first) {
        firstOp = first;
    }

    public int evaluate(int x, int y, int z) throws ParseException, OverflowException {
        return operation(firstOp.evaluate(x, y, z));
    }

    abstract protected int operation(int a) throws ParseException, OverflowException;
}
