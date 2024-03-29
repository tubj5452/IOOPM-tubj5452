package org.ioopm.calculator.tests;

import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;
import java.util.ArrayList;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.ioopm.calculator.ast.*; 
import org.ioopm.calculator.parser.*;
import org.ioopm.calculator.visitor.EvaluationVisitor;
import org.ioopm.calculator.visitor.NamedConstantChecker;
import org.ioopm.calculator.visitor.ReassignmentChecker; 

public class IntegrationTests {
    private CalculatorParser parser;
    private Environment env;
    private ScopeStack stack;  

    @BeforeAll
    static void initAll() {
    }

    @BeforeEach
    void init() {
        env = new Environment();
        stack = new ScopeStack(env); 
        parser = new CalculatorParser();
    }

    void afterParseTest(SymbolicExpression arg, String strToParse) {
        SymbolicExpression e1 = null; 
        try {
            e1 = parser.parse(strToParse, stack); 
            assertTrue(e1.equals(arg)); 
        } catch (IOException e) {
            fail("Unexpected exception was thrown");
        }
    }

    @Test
    void constantParserTest() {
        Constant c1 = new Constant(6); 
        String strC1 = c1.toString(); 
        String strInvalid = "6"; 

        assertEquals("6.0", strC1);
        assertNotEquals(strInvalid, strC1); 

        afterParseTest(c1, strC1);
        afterParseTest(c1, strInvalid);
    }

    @Test
    void namedConstantParserTest() {
        assertEquals(42.0, Constants.namedConstants.get("Answer"));
        afterParseTest(new Constant(Constants.namedConstants.get("Answer")), "42");
    }

    @Test
    void variableParserTest() {
        Variable v1 = new Variable("x"); 
        String strV1 = v1.toString(); 

        assertEquals("x", strV1);
        assertNotEquals("x", v1);

        afterParseTest(v1, strV1);

        Variable v2 = new Variable("dlefjwowknoirjewiu948r9w"); 
        String strV2 = v2.toString(); 

        assertEquals("dlefjwowknoirjewiu948r9w", strV2);

        afterParseTest(v2, strV2);

        assertThrows(RuntimeException.class, () -> {
            parser.parse("teststr|ngwith,symb@ls", stack); 
        });
    }

    @Test
    void unaryParserTest() {
        Cos cos1 = new Cos(new Constant(0)); 
        String strC1 = cos1.toString(); 

        assertEquals("cos 0.0", strC1);
        afterParseTest(cos1, strC1);

        Negation n1 = new Negation(cos1); 
        String strN1 = n1.toString(); 
        assertEquals("- (cos 0.0)", strN1);
        afterParseTest(n1, strN1);

        Constant c1 = new Constant(4); 
        Negation n2 = new Negation(c1); 
        String strN2 = n2.toString(); 
        assertEquals("- 4.0", strN2);
        afterParseTest(n2, strN2);

        Negation n3 = new Negation(new Negation(new Negation(c1))); 
        String strN3 = n3.toString(); 
        assertEquals("- (- (- 4.0))", strN3);
        afterParseTest(n3, strN3);
    }

    @Test 
    void binaryParserTest() {
        SymbolicExpression s1 = new Addition(new Constant(42), new Negation(new Constant(4711))); 
        String strS1 = s1.toString(); 
        assertEquals("42.0 + - 4711.0", strS1);
        afterParseTest(s1, strS1);

        SymbolicExpression s2 = new Multiplication(new Addition(new Constant(4), new Variable("y")), new Subtraction(new Constant(9), new Constant(7))); 
        String strS2 = s2.toString(); 
        assertEquals("(4.0 + y) * (9.0 - 7.0)", strS2);
        afterParseTest(s2, strS2);
    }

    @Test
    void parenthesesParserTest() {
        Assignment a1 = new Assignment(new Constant(1), new Variable("x")); 
        String strA1 = a1.toString(); 
        String strS1 = "(1.0 = x)"; 

        assertNotEquals(strA1, strS1);
        assertEquals("1.0 = x", strA1);

        afterParseTest(a1, strA1);
        afterParseTest(a1, strS1);

        assertThrows(RuntimeException.class, () -> {
            parser.parse("(x", stack); 
        });
    }

    @Test
    void whiteSpaceParserTest() {
        Addition a1 = new Addition(new Variable("x"), new Constant(3)); 
        String strA1 = a1.toString(); 
        String extraWiteSpace = "x     + 3.0   "; 

        assertNotEquals(strA1, extraWiteSpace);
        assertEquals("x + 3.0", strA1);
        assertNotEquals("x + 3.0", extraWiteSpace);

        afterParseTest(a1, strA1);
        afterParseTest(a1, extraWiteSpace);
    }

    @Test
    void complexExpTest() {
        SymbolicExpression s2 = new Division(new Multiplication(new Division(new Addition(new Constant(5), new Variable("x")), new Constant(2)), new Constant(1)), new Subtraction(new Constant(9), new Sin(new Constant(9)))); 
        String strS2 = s2.toString(); 

        assertEquals("(5.0 + x) / 2.0 * 1.0 / (9.0 - sin 9.0)", strS2);
        afterParseTest(s2, strS2);
    }

    @Test
    void namedConstantCheckerTest() throws IOException {
        SymbolicExpression s1 = new Addition(new Assignment(new Constant(2), new Variable("pi")), new Assignment(new Constant(42), new Variable("L"))); 
        String strS1 = s1.toString(); 
        SymbolicExpression e1 = parser.parse(strS1, stack); 
        assertTrue(e1.equals(s1)); 

        NamedConstantChecker NCchecker1 = new NamedConstantChecker(); 
        boolean noIllegalAssignments1 = NCchecker1.check(e1); 
        assertFalse(noIllegalAssignments1);
       
        SymbolicExpression s2 = new Addition(new Assignment(new Constant(2), new Variable("x")), new Constant(7)); 
        String strS2 = s2.toString(); 
        e1 = parser.parse(strS2, stack); 
        assertTrue(e1.equals(s2)); 

        NamedConstantChecker NCchecker2 = new NamedConstantChecker(); 
        boolean noIllegalAssignments2 = NCchecker2.check(e1); 
        assertTrue(noIllegalAssignments2);
    }

    @Test
    void ReassignmentCheckerTest() throws IOException {
        ReassignmentChecker Rchecker = new ReassignmentChecker(stack);
        EvaluationVisitor evaluator = new EvaluationVisitor();   
        
        SymbolicExpression s1 = new Assignment(new Constant(2), new Variable("x")); 
        String strS1 = s1.toString(); 
        SymbolicExpression e1 = parser.parse(strS1, stack);
        assertTrue(Rchecker.check(e1)); 
        evaluator.evaluate(e1, stack);

        Rchecker = new ReassignmentChecker(stack);
        SymbolicExpression s2 = new Assignment(new Constant(4), new Variable("x")); 
        String strS2 = s2.toString(); 
        e1 = parser.parse(strS2, stack); 
        assertTrue(Rchecker.check(e1));

        Rchecker = new ReassignmentChecker(stack);
        SymbolicExpression s3 = new Addition(new Assignment(new Constant(2), new Variable("y")), new Assignment(new Constant(6), new Variable("y"))); 
        String strS3 = s3.toString(); 
        e1 = parser.parse(strS3, stack); 
        assertFalse(Rchecker.check(e1));

        Rchecker = new ReassignmentChecker(stack);
        SymbolicExpression s4 = new Assignment(new Constant(4), new Variable("y")); 
        String strS4 = s4.toString(); 
        e1 = parser.parse(strS4, stack); 
        assertTrue(Rchecker.check(e1));
        evaluator.evaluate(e1, stack);

        Rchecker = new ReassignmentChecker(stack);
        SymbolicExpression s5 = new Addition(new Assignment(new Constant(10), new Variable("x")), new Assignment(new Constant(4), new Variable("x"))); 
        String strS5 = s5.toString(); 
        e1 = parser.parse(strS5, stack);
        assertFalse(Rchecker.check(e1)); 
        NamedConstantChecker NCchecker = new NamedConstantChecker(); 
        boolean noIllegalAssignments = NCchecker.check(e1); 
        assertTrue(noIllegalAssignments);
    }

    @Test
    void scopeTest() throws IOException {
        EvaluationVisitor evaluator = new EvaluationVisitor(); 

        SymbolicExpression e1 = parser.parse("{{1 = x} = x}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(1)));

        e1 = parser.parse("{(2 = x) + {1 = x}}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(3)));
    
        e1 = parser.parse("{{1 = x} = x} = y", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(1))); 
         
        e1 = parser.parse("x", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Variable("x")));

        e1 = parser.parse("y", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(1))); 

        e1 = parser.parse("{1 = x} + {1 = x}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(2)));

        e1 = parser.parse("{1 = x} + {x}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Addition(new Constant(1), new Variable("x"))));
        
        e1 = parser.parse("{1 = x} + {1}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(2)));  

        e1 = parser.parse("(1 = x) + {(2 + x = x) + {3 + x = x}}", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(10))); 

        e1 = parser.parse("x", stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(1))); 
    }

    @Test
    void conditionalTest() throws IOException{
        EvaluationVisitor evaluator = new EvaluationVisitor(); 
        SymbolicExpression s1 = new Assignment(new Constant(2), new Variable("x")); 
        SymbolicExpression s2 = new Assignment(new Constant(4), new Variable("y")); 

        SymbolicExpression e1 = parser.parse(s1.toString(), stack); 
        SymbolicExpression lhsEval = evaluator.evaluate(e1, stack); 
        assertTrue(lhsEval.equals(new Constant(2)));

        e1 = parser.parse(s2.toString(), stack); 
        SymbolicExpression rhsEval = evaluator.evaluate(e1, stack); 
        assertTrue(rhsEval.equals(new Constant(4)));

        SymbolicExpression c = new Conditional("<", lhsEval, rhsEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack);
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(42)));

        c = new Conditional(">", lhsEval, rhsEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(42)));

        c = new Conditional(">=", lhsEval, rhsEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(42)));

        SymbolicExpression s3 = new Assignment(new Constant(2), new Variable("y")); 
        SymbolicExpression rhsEqualEval = evaluator.evaluate(parser.parse(s3.toString(), stack), stack); 
        c = new Conditional(">=", lhsEval, rhsEqualEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(42)));

        c = new Conditional("<=", lhsEval, rhsEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(42)));
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(4711)));
        c = new Conditional("<=", lhsEval, rhsEqualEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(42)));
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(4711)));

        c = new Conditional("==", lhsEval, rhsEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(42)));
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(4711)));
        c = new Conditional("==", lhsEval, rhsEqualEval, new Scope(new Constant(42)), new Scope(new Constant(4711))); 
        e1 = parser.parse(c.toString(), stack); 
        assertTrue(evaluator.evaluate(e1, stack).equals(new Constant(42)));
        assertFalse(evaluator.evaluate(e1, stack).equals(new Constant(4711)));
    }

    @Test
    void functionTest() throws IOException {
        EvaluationVisitor evaluator = new EvaluationVisitor(); 

        SymbolicExpression fd1 = parser.parse("function max(x, y)", stack);
        FunctionDeclaration fdExpression = fd1.getFunctionDeclaration(); 
        Variable funcName = fd1.getFuncName(); 

        stack.getLastEnv().put(funcName, fdExpression);
        fd1 = parser.parse("if x < y { y } else { x }", stack); 
        fdExpression.getSequence().addExpression(fd1); 
        
        ArrayList<Atom> arguments =  new ArrayList<>();
        arguments.add(new Constant(4));
        arguments.add(new Constant(1)); 
        SymbolicExpression fc1 = new FunctionCall(new Variable("max"), arguments); 
        
        SymbolicExpression fcEval1 = parser.parse("max(1, 4)", stack); 

        SymbolicExpression max_result = evaluator.evaluate(fcEval1, stack); 
       
        assertTrue(max_result.equals(new Constant(4)));

        ArrayList<Variable> paramaters = new ArrayList<>(); 
        paramaters.add(new Variable("x")); 
        Sequence body = new Sequence(); 
        body.addExpression(new Assignment(new Multiplication(new Variable("x"), new Constant(3)), new Variable("y")));
        body.addExpression(new Addition(new Variable("x"), new Variable("y"))); 
        FunctionDeclaration fd2 = new FunctionDeclaration(new Variable("timesthree"), paramaters, body); 
        
        stack.getLastEnv().put(new Variable("timesthree"), fd2);

        ArrayList<Atom> argument =  new ArrayList<>();
        argument.add(new Constant(4));
        FunctionCall fc2 = new FunctionCall(new Variable("timesthree"), argument); 

        SymbolicExpression fcParsed = parser.parse(fc2.toString(), stack); 
        
        SymbolicExpression fcEval2 = evaluator.evaluate(fcParsed, stack); 
        assertTrue(fcEval2.equals(new Constant(16))); 
    }

    @AfterEach
    void tearDown() {

    }

    @AfterAll
    static void tearDownAll() {
    }
}
