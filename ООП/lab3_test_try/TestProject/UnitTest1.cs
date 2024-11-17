//using Microsoft.VisualStudio.TestTools.UnitTesting;
//using System.Collections.Generic;
//using System.Linq;

//[TestClass]
//public class ExpressionTests
//{
//    static LogFun Log(Expr left, Expr right) => new LogFun(left, right);
//    static PowFun Pow(Expr left, Expr right) => new PowFun(left, right);
//    static SqrtFun Sqrt(Expr operand) => new SqrtFun(operand);
//    [TestMethod]
//    public void TestPowerFunctionWithVariable()
//    {
//        var x = new Variable("x");
//        var expr1 = Pow(x, 3); // выражение: x^3

//        Assert.AreEqual("(x ^ 3)", expr1.ToString());
//        CollectionAssert.AreEquivalent(new List<string> { "x" }, expr1.Variables.ToList());
//        Assert.IsFalse(expr1.IsConstant);
//        Assert.IsTrue(expr1.IsPolynomial);
//        Assert.AreEqual(3, expr1.PolynomialDegree);
//        Assert.AreEqual(8.0, expr1.Compute(new Dictionary<string, double> { ["x"] = 2 }));
//    }

//    [TestMethod]
//    public void TestLogFunction()
//    {
//        var x = new Variable("x");
//        var expr2 = Log(2, 8); // выражение: log_2(8)

//        Assert.AreEqual("Log2(8)", expr2.ToString());
//        CollectionAssert.AreEquivalent(new List<string>(), expr2.Variables.ToList());
//        Assert.IsTrue(expr2.IsConstant);
//        Assert.IsTrue(expr2.IsPolynomial);
//        Assert.AreEqual(0, expr2.PolynomialDegree);
//        Assert.AreEqual(3.0, expr2.Compute(new Dictionary<string, double>()));
//    }

//    [TestMethod]
//    public void TestPowerAndDivisionFunction()
//    {
//        var x = new Variable("x");
//        var y = new Variable("y");
//        var expr3 = Pow(x, 4) / Pow(y, 2); // выражение: (x^4) / (y^2)

//        Assert.AreEqual("((x ^ 4) / (y ^ 2))", expr3.ToString());
//        CollectionAssert.AreEquivalent(new List<string> { "x", "y" }, expr3.Variables.ToList());
//        Assert.IsFalse(expr3.IsConstant);
//        Assert.IsFalse(expr3.IsPolynomial);
//        Assert.AreEqual(0, expr3.PolynomialDegree);
//        Assert.AreEqual(64.0, expr3.Compute(new Dictionary<string, double> { ["x"] = 4, ["y"] = 2 }));
//    }
//}



using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class ExprTests
{
    static LogFun Log(Expr left, Expr right) => new LogFun(left, right);
    static PowFun Pow(Expr left, Expr right) => new PowFun(left, right);
    static SqrtFun Sqrt(Expr operand) => new SqrtFun(operand);
    [TestMethod]
    public void Test_Pow_Function_With_Variable()
    {
        // Arrange
        var x = new Variable("x");
        var expr = Pow(x, 3);

        // Act
        var resultString = expr.ToString();
        var variables = expr.Variables.Select(v => $"\"{v}\"").ToArray();
        var isConstant = expr.IsConstant;
        var isPolynomial = expr.IsPolynomial;
        var polynomialDegree = expr.PolynomialDegree;
        var computedResult = expr.Compute(new Dictionary<string, double> { ["x"] = 2 });

        // Assert
        Assert.AreEqual("(x ^ 3)", resultString);
        CollectionAssert.AreEqual(new[] { "\"x\"" }, variables);
        Assert.IsFalse(isConstant);
        Assert.IsTrue(isPolynomial);
        Assert.AreEqual(3, polynomialDegree);
        Assert.AreEqual(8.0, computedResult, 1e-6);
    }

    [TestMethod]
    public void Test_Log_Function_With_Constant_Base()
    {
        // Arrange
        var x = new Variable("x");
        var expr = Log(new Constant(2), x);

        // Act
        var resultString = expr.ToString();
        var variables = expr.Variables.Select(v => $"\"{v}\"").ToArray();
        var isConstant = expr.IsConstant;
        var isPolynomial = expr.IsPolynomial;
        var polynomialDegree = expr.PolynomialDegree;
        var computedResult = expr.Compute(new Dictionary<string, double> { ["x"] = 8 });

        // Assert
        Assert.AreEqual("Log2(x)", resultString);
        CollectionAssert.AreEqual(new[] { "\"x\"" }, variables);
        Assert.IsFalse(isConstant);
        Assert.IsFalse(isPolynomial);
        Assert.AreEqual(0, polynomialDegree);
        Assert.AreEqual(3.0, computedResult, 1e-6);
    }

    [TestMethod]
    public void Test_Log_Function_With_Constant_Base_And_Constant()
    {
        // Arrange
        var c = new Constant(8);
        var expr = Log(2, 8);

        // Act
        var resultString = expr.ToString();
        //var variables = expr.Variables.Select(v => $"\"{v}\"").ToArray();
        var isConstant = expr.IsConstant;
        var isPolynomial = expr.IsPolynomial;
        var polynomialDegree = expr.PolynomialDegree;
        //var computedResult = expr.Compute(new Dictionary<string, double> { ["x"] = 8 });

        // Assert
        Assert.AreEqual("Log2(8)", resultString);
        //CollectionAssert.AreEqual(new[] { "" }, variables);
        Assert.IsTrue(isConstant);
        Assert.IsTrue(isPolynomial);
        Assert.AreEqual(0, polynomialDegree);
        //Assert.AreEqual(3.0, computedResult, 1e-6);
    }

    [TestMethod]
    public void Test_Sqrt_Function_With_Constant()
    {
        // Arrange
        var c = new Constant(16);
        var expr = Sqrt(c);

        // Act
        var resultString = expr.ToString();
        var variables = expr.Variables.Select(v => $"\"{v}\"").ToArray();
        var isConstant = expr.IsConstant;
        var isPolynomial = expr.IsPolynomial;
        var polynomialDegree = expr.PolynomialDegree;
        var computedResult = expr.Compute(new Dictionary<string, double>());

        // Assert
        Assert.AreEqual("Sqrt(16)", resultString);
        CollectionAssert.AreEqual(new string[0], variables);
        Assert.IsTrue(isConstant);
        Assert.IsTrue(isPolynomial);
        Assert.AreEqual(0, polynomialDegree);
        Assert.AreEqual(4.0, computedResult, 1e-6);
    }

    [TestMethod]
    public void Test_Add_Function_With_Variables()
    {
        // Arrange
        var x = new Variable("x");
        var y = new Variable("y");
        var expr = x + y;

        // Act
        var resultString = expr.ToString();
        var variables = expr.Variables.Select(v => $"\"{v}\"").ToArray();
        var isConstant = expr.IsConstant;
        var isPolynomial = expr.IsPolynomial;
        var polynomialDegree = expr.PolynomialDegree;
        var computedResult = expr.Compute(new Dictionary<string, double> { ["x"] = 1, ["y"] = 2 });

        // Assert
        Assert.AreEqual("(x + y)", resultString);
        CollectionAssert.AreEqual(new[] { "\"x\"", "\"y\"" }, variables);
        Assert.IsFalse(isConstant);
        Assert.IsTrue(isPolynomial);
        Assert.AreEqual(1, polynomialDegree);
        Assert.AreEqual(3.0, computedResult, 1e-6);
    }
}
