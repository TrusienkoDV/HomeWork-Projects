package expression;

public class Const implements TripleExpression {
    private double valI;

    public Const(int val) {
        this.valI = val;
    }

    public int evaluate(int x, int y, int z) {
        return (int) valI;
    }
}