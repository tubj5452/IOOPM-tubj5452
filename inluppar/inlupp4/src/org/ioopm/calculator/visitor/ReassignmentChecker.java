package org.ioopm.calculator.visitor;

import org.ioopm.calculator.ast.*;
import java.util.HashSet;

/**
 * Visitor for checking SymbolicExpressions.
 */
public class ReassignmentChecker implements Visitor {
    private HashSet<Variable> currentAssignments;  
    private boolean variableInUse; 
    private Variable lastReassigned;
    private boolean reassignedDuringCheck;  

    /**
     * Constructs a ReassignmentChecker as an array, a control if a
     * variable is in use, the last variable reassigned and a control
     * if a reassignment happens in the current expression.  
     */
    public ReassignmentChecker() {
        this.currentAssignments = new HashSet<>(); 
        this.variableInUse = true; 
        this.lastReassigned = null; 
        this.reassignedDuringCheck = false; 
    }

    /**
     * Gets the variable that was last reassigned. 
     *
     * @return The variable last reassigned. 
     */
    public Variable getLastReassigned() {
        return this.lastReassigned; 
    }

    /**
     * Checks a top-level SymbolicExpression for reassigned variables.
     * If reassignment happens in the top-level expression it is removed
     * from the set of variables currently in use. 
     *
     * @param topLevel The SymbolicExpression to check.
     * @return True if no reassignments are found, false otherwise.
     */
    public boolean check(SymbolicExpression topLevel, Environment vars) {
        this.variableInUse = true;    
        this.reassignedDuringCheck = false; 

        topLevel.accept(this);

        if (reassignedDuringCheck) {
            currentAssignments.removeIf(variable -> !vars.containsKey(variable));
        }

        return this.variableInUse; 
    }

    /**
     * Visits an Addition node, checking its left and right subexpressions.
     *
     * @param a The Addition node to visit.
     * @return null
     */
    public SymbolicExpression visit(Addition a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    /**
     * Visits an Assignment node, checking its left and right subexpressions.
     * If the right-hand side does not exist in the hashset of variables in 
     * use it adds the Varibale to it. 
     *
     * @param a The Assignment node to visit.
     * @return null
     */
    public SymbolicExpression visit(Assignment a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);        

        if (!a.getRhs().isConstant()) {
            if (!currentAssignments.contains(a.getRhs())) {
                currentAssignments.add(a.getRhs().getVariable()); 
            } else {
                this.variableInUse = false; 
                this.reassignedDuringCheck = true; 
                this.lastReassigned = a.getRhs().getVariable(); 
            }
        }
        return null;
    }

    /**
     * Visits a Clear node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(Clear a) {
        return null; 
    }

    /**
     * Visits a Constant node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(Constant a) {
        return null; 
    }

    /**
     * Visits a Cos node, checking its argument subexpressions.
     *
     * @param a The Cos node to visit.
     * @return null
     */
    public SymbolicExpression visit(Cos a) {
        a.getArg().accept(this); 
        return null; 
    }

    /**
     * Visits a Division node, checking its left and right subexpressions.
     *
     * @param a The Division node to visit.
     * @return null
     */
    public SymbolicExpression visit(Division a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    /**
     * Visits an Exp node, checking its argument subexpressions.
     *
     * @param a The Exp node to visit.
     * @return null
     */
    public SymbolicExpression visit(Exp a) {
        a.getArg().accept(this); 
        return null;
    }

    /**
     * Visits a Log node, checking its argument subexpressions.
     *
     * @param a The Log node to visit.
     * @return null
     */
    public SymbolicExpression visit(Log a) {
        a.getArg().accept(this); 
        return null;
    }

    /**
     * Visits a Multiplication node, checking its left and right subexpressions.
     *
     * @param a The Multiplication node to visit.
     * @return null
     */
    public SymbolicExpression visit(Multiplication a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    /**
     * Visits a NamedConstant node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(NamedConstant a) {
        return null; 
    }

    /**
     * Visits a Negation node, checking its argument subexpressions.
     *
     * @param a The Negation node to visit.
     * @return null
     */
    public SymbolicExpression visit(Negation a) {
        a.getArg().accept(this); 
        return null;
    }

    /**
     * Visits a Quit node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(Quit a) {
        return null; 
    }

    /**
     * Visits a Sin node, checking its argument subexpressions.
     *
     * @param a The Sin node to visit.
     * @return null
     */
    public SymbolicExpression visit(Sin a) {
        a.getArg().accept(this); 
        return null;
    }

    /**
     * Visits a Subtraction node, checking its left and right subexpressions.
     *
     * @param a The Subtraction node to visit.
     * @return null
     */
    public SymbolicExpression visit(Subtraction a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    /**
     * Visits a Variable node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(Variable a) {
        return null; 
    }

    /**
     * Visits a Vars node, not checking anything
     * 
     * @return null
     */
    public SymbolicExpression visit(Vars a) {
        return null; 
    }
}
