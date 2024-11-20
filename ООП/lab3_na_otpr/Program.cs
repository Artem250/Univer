using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Dynamic;
using System.Reflection.Emit;
using System.Reflection.Metadata;
using System.Runtime.CompilerServices;
using System.Security.AccessControl;

static LogFun Log(Expr left, Expr right) => new(left, right);
static PowFun Pow(Expr left, Expr right) => new(left, right);
static SqrtFun Sqrt(Expr operand) => new(operand);

//var x = new Variable("x");
//var y = new Variable("y");
//var z = new Variable("z");
//var w = new Variable("w");
//var c = new Constant(3);
////var expr1 = (x - 4) * (3 * x + y * y) / 5;
////var expr2 = (5 - 3 * c) * Sqrt(16 + c * c);


////Console.WriteLine(expr1.ToString());
////Console.WriteLine(expr1.Variables);
////Console.WriteLine(expr1.IsConstant);
////Console.WriteLine(expr1.IsPolynomial);
////Console.WriteLine(expr1.PolynomialDegree);
////Console.WriteLine(expr1.Compute(new Dictionary<string, double>
////{ ["x"] = 1, ["y"] = 2 }));
////Console.WriteLine(expr2.ToString());
////Console.WriteLine(expr2.Variables);
////Console.WriteLine(expr2.IsConstant);
////Console.WriteLine(expr2.IsPolynomial);
////Console.WriteLine(expr2.PolynomialDegree);
////Console.WriteLine(expr2.Compute(new Dictionary<string, double>
////{ ["x"] = 1, ["y"] = 2 }));


//var expr3 = x * c;
//var expr1 = (x - 4) * (3 * x + y * y) / 5;
//var expr2 = (5 - 3 * c) * Sqrt(16 + c * c);
//var expr4 = Sqrt(2 * x + 4);


//Console.WriteLine(expr1.ToString());
//Console.WriteLine($"[{string.Join(", ", expr1.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr1.IsConstant);
//Console.WriteLine(expr1.IsPolynomial);
//Console.WriteLine(expr1.PolynomialDegree);
//Console.WriteLine(expr1.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));
//Console.WriteLine("\n");

//Console.WriteLine(expr2.ToString());
//Console.WriteLine($"[{string.Join(", ", expr2.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr2.IsConstant);
//Console.WriteLine(expr2.IsPolynomial);
//Console.WriteLine(expr2.PolynomialDegree);
//Console.WriteLine(expr2.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));
//Console.WriteLine("\n");

//Console.WriteLine(expr3.ToString());
//Console.WriteLine($"[{string.Join(", ", expr3.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr3.IsConstant);
//Console.WriteLine(expr3.IsPolynomial);
//Console.WriteLine(expr3.PolynomialDegree);
//Console.WriteLine(expr3.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));
//Console.WriteLine("\n");

//Console.WriteLine(expr4.ToString());
//Console.WriteLine($"[{string.Join(", ", expr4.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr4.IsConstant);
//Console.WriteLine(expr4.IsPolynomial);
//Console.WriteLine(expr4.PolynomialDegree);
//Console.WriteLine(expr4.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));





var x = new Variable("x");
//var y = new Variable("y");
var y = 5 * x;
var c = new Constant(5);
//var expr1 = Sqrt(2 * x + Pow(y, 2)) / Log(2, x - 1);
//var expr1 = 5 / Log(2, x - 1);
var expr1 = -x;
var expr2 = (Pow(x, 3) - 3 * x + c) * (Pow(y, 2) + c);

// Проверка вычисления
Console.WriteLine($"Expr1: {expr1}");
Console.WriteLine($"Expr1 Value (x=2, y=4): {expr1.Compute(new Dictionary<string, double> { ["x"] = 4, ["y"] = 4 })}");

Console.WriteLine($"Expr2: {expr2}");
Console.WriteLine($"Expr2 Value (x=2, y=1): {expr2.Compute(new Dictionary<string, double> { ["x"] = 2, ["y"] = 1 })}");

// Проверка свойств
Console.WriteLine($"Expr1 Variables: [{string.Join(", ", expr1.Variables)}]");
Console.WriteLine($"Expr1 Is Polynomial: {expr1.IsPolynomial}");
Console.WriteLine($"Expr1 Polynomial Degree: {expr1.PolynomialDegree}");

Console.WriteLine($"Expr2 Variables: [{string.Join(", ", expr2.Variables)}]");
Console.WriteLine($"Expr2 Is Polynomial: {expr2.IsPolynomial}");
Console.WriteLine($"Expr2 Polynomial Degree: {expr2.PolynomialDegree}");

// Производные
var derivative1 = expr1.Derivative(x);
Console.WriteLine($"Derivative of Expr1 w.r.t x: {derivative1}");

var derivative2 = expr2.Derivative(x);
Console.WriteLine($"Derivative of Expr2 w.r.t x: {derivative2}");

Console.WriteLine($"Derivative of Expr1 Value (x=2, y=1): {derivative1.Compute(new Dictionary<string, double> { ["x"] = 4, ["y"] = 1 })}");
Console.WriteLine($"Derivative of Expr2 Value (x=2, y=1): {derivative2.Compute(new Dictionary<string, double> { ["x"] = 2, ["y"] = 1 })}");






//var expr1 = Log(x, x);
////var expr1 = 5;
////var expr1 = Pow(x, 2) + Log(x, 8);
////var expr1 = Pow(x, 3.2);
////var expr1 = Log(Math.E, x);
//var expr2 = expr1.Derivative(x);

//Console.WriteLine(expr2.Compute(new Dictionary<string, double>
//{ ["x"] = 2, ["y"] = 1 }));
//Console.WriteLine("IsConstant: " + expr2.IsConstant);

//var expr1 = (5 - 3 * c) * Sqrt(16 + c * c);


//var expr2 = Log(2, 8.0);

//var expr3 = Pow(x, 4) / Pow(y, 2); //+ (2 * Pow(y, 2)) - (5 * Pow(x, 2)) - Pow(y, 2);
//var expr3 = Pow(2, x) + x;
//Console.WriteLine(Math.Pow(2, 2));



//Console.WriteLine(expr1.ToString());
//Console.WriteLine($"[{string.Join(", ", expr1.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine("IsConstant: " + expr1.IsConstant);
//Console.WriteLine("IsPolynomial: " + expr1.IsPolynomial);
//Console.WriteLine("PolynomialDegree: " + expr1.PolynomialDegree);
//Console.WriteLine(expr1.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));
//Console.WriteLine(expr1.Derivative(x));



//Console.WriteLine("-----------------------------------");
//Console.WriteLine(expr2.ToString());
//Console.WriteLine($"[{string.Join(", ", expr2.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr2.IsConstant);
//Console.WriteLine(expr2.IsPolynomial);
//Console.WriteLine("PolynomialDegree: " + expr2.PolynomialDegree);
//Console.WriteLine(expr2.Compute(new Dictionary<string, double>
//{ ["x"] = 2, ["y"] = 2 }));

//Console.WriteLine("-----------------------------------");

//Console.WriteLine(expr3.ToString());
//Console.WriteLine($"[{string.Join(", ", expr3.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr3.IsConstant);
//Console.WriteLine(expr3.IsPolynomial);
//Console.WriteLine("PolynomialDegree: " + expr3.PolynomialDegree);

//Console.WriteLine(expr3.Compute(new Dictionary<string, double>
//{ ["x"] = 2, ["y"] = 2 }));

public interface IExpr
{
    double Compute(IReadOnlyDictionary<string, double> variableValues);
    IEnumerable<string> Variables { get; }
    bool IsConstant { get; }
    bool IsPolynomial { get; }
    int PolynomialDegree { get; }
}

public abstract class Expr : IExpr
{
    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public abstract IEnumerable<string> Variables { get; }
    public abstract bool IsConstant { get; }
    public abstract bool IsPolynomial { get; }
    public abstract int PolynomialDegree { get; }

    public abstract Expr Derivative(Variable x);


    public static Negative operator -(Expr operand) => new Negative(operand);
    public static Add operator +(Expr left, Expr right) => new(left, right);
    public static Sub operator -(Expr left, Expr right) => new(left, right);
    public static Mult operator *(Expr left, Expr right) => new(left, right);
    public static Div operator /(Expr left, Expr right) => new(left, right);
    public static implicit operator Expr(int value) => new Constant(value);
    public static implicit operator Expr(double value) => new Constant(value);
}

public abstract class UnaryOperation : Expr
{
    protected Expr Operand { get; }
    protected UnaryOperation(Expr operand) { Operand = operand; }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant { get; }
    public override bool IsPolynomial { get; }
    public override int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);

}

public abstract class BinaryOperation : Expr
{
    protected Expr Left { get; }
    protected Expr Right { get; }
    protected BinaryOperation(Expr left, Expr right) { Left = left; Right = right; }
    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant { get; }
    public override bool IsPolynomial { get; }
    public override int PolynomialDegree { get; }
}

public abstract class Function : Expr
{
    protected Expr Left { get; }
    protected Expr Right { get; }
    protected Function(Expr left, Expr right) { Left = left; Right = right; }
    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant { get; }
    public override bool IsPolynomial { get; }
    public override int PolynomialDegree { get; }

}


public class Constant : Expr
{
    public object Value { get; }

    public Constant(double value) { Value = value; }
    public Constant(int value) { Value = value; }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Value is double dValue ? dValue : (int)Value;

    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
    public override bool IsConstant => true;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 0;

    public override string ToString() => Value.ToString();
    public override Expr Derivative(Variable x) => 0;
}


public class Variable : Expr
{
    public string Name { get; }
    public Variable(string name) { Name = name; }
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => variableValues[Name];
    public override IEnumerable<string> Variables => new List<string> { Name };
    public override bool IsConstant => false;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 1;
    public override string ToString() => Name;
    public override Expr Derivative(Variable x) => x == this ? 1 : 0;

}

public class Negative : UnaryOperation
{
    public Negative(Expr operand) : base(operand) { }

    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => Operand.IsPolynomial;
    public override int PolynomialDegree => Operand.PolynomialDegree;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => (-1) * Operand.Compute(variableValues);
    public override string ToString() => $"(-{Operand})";
    public override Expr Derivative(Variable x) => -1 * Operand.Derivative(x);

}

public class Add : BinaryOperation
{
    public Add(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) + Right.Compute(variableValues);
    }
    public override string ToString() => $"({Left} + {Right})";
    public override Expr Derivative(Variable x) => Left.Derivative(x) + Right.Derivative(x);
}

public class Sub : BinaryOperation
{
    public Sub(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) - Right.Compute(variableValues);
    }
    public override string ToString() => $"({Left} - {Right})";
    public override Expr Derivative(Variable x) => Left.Derivative(x) - Right.Derivative(x);

}

public class Mult : BinaryOperation
{
    public Mult(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => (Left.PolynomialDegree + Right.PolynomialDegree);
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) * Right.Compute(variableValues);
    }
    public override string ToString() => $"({Left} * {Right})";
    public override Expr Derivative(Variable x) => Left.Derivative(x) * Right + Left * Right.Derivative(x);

}


public class Div : BinaryOperation
{
    public Div(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsConstant;
    public override int PolynomialDegree => IsPolynomial ? Left.PolynomialDegree - Right.PolynomialDegree : -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        var rightConst = Right.Compute(variableValues);
        if (rightConst == 0.0)
            throw new ArgumentException("Division by 0");

        return Left.Compute(variableValues) / rightConst;
    }

    public override string ToString() => $"({Left} / {Right})";
    public override Expr Derivative(Variable x) => (Left.Derivative(x) * Right - Left * Right.Derivative(x)) / (Right * Right);

}

public class PowFun : Function
{

    public PowFun(Expr left, Expr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Right is Constant rightConst && rightConst.Value is int;
    public override int PolynomialDegree =>
        Left.IsConstant ? 0 : (IsPolynomial ? Convert.ToInt32(((Constant)Right).Value) : -1);

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Math.Pow(Left.Compute(variableValues), Right.Compute(variableValues));
    }

    public override string ToString() => $"({Left} ^ {Right})";


    public override Expr Derivative(Variable x)
    {
        return new PowFun(Left, Right) * new LogFun(Math.E, Left) * Right.Derivative(x) +
            new PowFun(Left, Right) / Left * Left.Derivative(x) * Right;
    }
}

public class LogFun : Function
{
    public LogFun(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsConstant && Right.IsConstant;
    public override int PolynomialDegree => IsConstant ? 0 : -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        double leftVal = Left.Compute(variableValues);
        double rightVal = Right.Compute(variableValues);
        if (leftVal <= 0 || leftVal == 1 || rightVal <= 0) throw new Exception("The logarithm makes sense when a > 0, a != 1, b > 0");

        return Math.Log(Right.Compute(variableValues), Left.Compute(variableValues));
    }
    public override string ToString() => $"Log({Left}; {Right})";


    public override Expr Derivative(Variable x) => (Left * new LogFun(Math.E, Left) * Right.Derivative(x) -
        Left.Derivative(x) * Right * new LogFun(Math.E, Right)) /
        (Left * new PowFun(new LogFun(Math.E, Left), 2) * Right);
}

public class SqrtFun : UnaryOperation
{
    public SqrtFun(Expr operand) : base(operand) { }
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => Operand.IsConstant;
    public override int PolynomialDegree => IsConstant ? 0 : -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        double val = Operand.Compute(variableValues);
        if (val < 0) throw new Exception("The square root of a negative number cannot be calculated");
        return Math.Sqrt(val);
    }
    public override string ToString() => $"Sqrt({Operand})";
    public override Expr Derivative(Variable x) => (0.5) * Operand.Derivative(x) / new SqrtFun(Operand);
}

