package org.ioopm.calculator.visitor;

import org.ioopm.calculator.ast.*;

import java.util.ArrayList;
import java.util.List;

/**
 * Visitor for checking SymbolicExpressions.
 */
public class ReassignmentChecker implements Visitor {
    private ScopeStack stack;
    private List<Variable> currentAssignments;
    private boolean variableNotInUse; 
    private Variable lastReassigned;

    /**
     * Constructs a ReassignmentChecker as an array, a control if a
     * variable is in use, the last variable reassigned.
     */
    public ReassignmentChecker(ScopeStack stack) {
        this.stack = stack;
        this.currentAssignments = new ArrayList<>(); 
        this.variableNotInUse = true; 
        this.lastReassigned = null; 
    }

    /**
     * Gets the list of reassigned variables found during checking.
     *
     * @return The list of reassigned variables. 
     */
    public List<Variable> getReassignments() {
        return currentAssignments;
    }

    /**
     * Checks a top-level SymbolicExpression for reassigned variables.
     *
     * @param topLevel The SymbolicExpression to check.
     * @return True if no reassignments are found, false otherwise.
     */
    public boolean check(SymbolicExpression topLevel) {
        this.variableNotInUse = true;

        topLevel.accept(this);

        return variableNotInUse; 
    }

    /**
     * Visits an Addition node, checking its left and right subexpressions.
     *
     * @param a The Addition node to visit.
     * @return null
     */
    @Override
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
    @Override
    public SymbolicExpression visit(Assignment a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);        

        if (!a.getRhs().isNamedConstant()) {
            if (!currentAssignments.contains(a.getRhs())) {
                currentAssignments.add(a.getRhs().getVariable()); 
            } else {
                this.variableNotInUse = false; 
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
    @Override
    public SymbolicExpression visit(Clear a) {
        return null; 
    }

    /**
     * Visits a Conditional node, not checking anything
     * 
     * @return null
     */
    @Override 
    public SymbolicExpression visit(Conditional a) {
        return null; 
    }

    /**
     * Visits a Constant node, not checking anything
     * 
     * @return null
     */
    @Override
    public SymbolicExpression visit(Constant a) {
        return null; 
    }

    /**
     * Visits a Cos node, checking its argument subexpressions.
     *
     * @param a The Cos node to visit.
     * @return null
     */
    @Override
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
    @Override
    public SymbolicExpression visit(Division a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    /**
     * Visits an End node, not checking anything
     * 
     * @return null
     */
    @Override
    public SymbolicExpression visit(End a) {
        return null; 
    }

    /**
     * Visits an Exp node, checking its argument subexpressions.
     *
     * @param a The Exp node to visit.
     * @return null
     */
    @Override
    public SymbolicExpression visit(Exp a) {
        a.getArg().accept(this); 
        return null;
    }

    public SymbolicExpression visit(FunctionCall a){
        a.getFuncName().accept(this);

        for (Atom argument : a.getArguments()) {
            argument.accept(this);
        }

        return null;
    }

    public SymbolicExpression visit(FunctionDeclaration a) {
        a.getFuncName().accept(this);

        for (Variable parameter : a.getParameters()) {
            parameter.accept(this);
        }

        a.getSequence().accept(this);

        return null;
    }

    /**
     * Visits a Log node, checking its argument subexpressions.
     *
     * @param a The Log node to visit.
     * @return null
     */
    @Override
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
    @Override
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
    @Override
    public SymbolicExpression visit(NamedConstant a) {
        return null; 
    }

    /**
     * Visits a Negation node, checking its argument subexpressions.
     *
     * @param a The Negation node to visit.
     * @return null
     */
    @Override
    public SymbolicExpression visit(Negation a) {
        a.getArg().accept(this); 
        return null;
    }

    /**
     * Visits a Quit node, not checking anything
     * 
     * @return null
     */
    @Override
    public SymbolicExpression visit(Quit a) {
        return null; 
    }

    /**
     * Visits a Scope node, and evaluates the SymbolicExpression wihtin it
     * @param n The Scope node to visit.
     * @return the result of the evaluated scope
    */
    @Override
    public SymbolicExpression visit(Scope a) {
        currentAssignments.clear();
        this.variableNotInUse = true;
        this.lastReassigned = null;

        SymbolicExpression exp = a.getScope();
        exp.accept(this);

        currentAssignments.clear();
        this.variableNotInUse = true;
        this.lastReassigned = null;
                
        return null;
    }

    
    public SymbolicExpression visit(Sequence a){
        for (SymbolicExpression expression : a.getBody()) {
            expression.accept(this);
        }

        return null; 
    } 

    /**
     * Visits a Sin node, checking its argument subexpressions.
     *
     * @param a The Sin node to visit.
     * @return null
     */
    @Override
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
    @Override
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
    @Override
    public SymbolicExpression visit(Variable a) {
        return null; 
    }

    /**
     * Visits a Vars node, not checking anything
     * 
     * @return null
     */
    @Override
    public SymbolicExpression visit(Vars a) {
        return null; 
    }
}
