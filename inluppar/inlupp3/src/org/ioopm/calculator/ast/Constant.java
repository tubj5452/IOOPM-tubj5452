package org.ioopm.calculator.ast;

/**
 * A subclass of Atom, representing a constant value in a symbolic expression.
 */
public class Constant extends Atom {
    private double value;

    /**
     * Constructs a Constant object with the given numeric value.
     *
     * @param value The numeric value of the constant.
     */
    public Constant(double value) {
        this.value = value;
    }

    /**
     * Returns true, indicating that the object is a constant.
     *
     * @return True.
     */
    @Override
    public boolean isConstant() {
        return true;
    }

    /**
     * Returns the numeric value of the constant.
     *
     * @return The numeric value of the constant.
     */
    @Override
    public double getValue() {
        return this.value;
    }

    /**
     * Returns the string representation of the constant.
     *
     * @return The string representation of the constant.
     */
    public String toString() {
        return String.valueOf(this.value);
    }

    /**
     * Checks if the specified object is instance of constant, to later 
     * check equality to the current object.      
     *
     * @param other The object to compare with.
     * @return True if the objects are equal, false otherwise.
     */
    public boolean equals(Object other) {
        if (other instanceof Constant) {
            return this.equals((Constant) other);
        } else {
            return false;
        }
    }

    /**
     * Checks if the current constant is equal to the specified constant.
     *
     * @param other The constant to compare with.
     * @return True if the constants are equal, false otherwise.
     */
    public boolean equals(Constant other) {
        return this.value == other.value;
    }

    /**
     * Evaluates the constant expression by returning the constant itself.
     *
     * @param vars The environment containing variable values.
     * @return SymbolicExpression The result of the constant evaluation.
     */
    public SymbolicExpression eval(Environment vars) {
        return new Constant(this.value);
    }
}
