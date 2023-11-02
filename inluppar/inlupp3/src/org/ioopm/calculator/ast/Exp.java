package org.ioopm.calculator.ast;

public class Exp extends Unary {

    public Exp(SymbolicExpression operand) {
        super(operand); 
    }
    
    public String getName() {
        return "e^"; 
    }
}