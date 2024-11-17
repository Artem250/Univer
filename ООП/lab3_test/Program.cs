using System;
using System.Collections.Generic;

public interface IExpr
{
    IEnumerable<string> Variables { get; }
    bool IsConstant { get; }
    bool IsPolynomial { get; }
    int PolynomialDegree { get; }
    double Compute(IReadOnlyDictionary<string, double> variableValues);
}

public abstract class BinaryOperation : IExpr
{
    protected IExpr Left;
    protected IExpr Right;

    protected BinaryOperation(IExpr left, IExpr right)
    {
        Left = left;
        Right = right;
    }

    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);

    public IEnumerable<string> Variables
    {
        get
        {
            var variables = new HashSet<string>(Left.Variables);
            foreach (var v in Right.Variables)
            {
                variables.Add(v);
            }
            return variables;
        }
    }

    public bool IsConstant => Left.IsConstant && Right.IsConstant;

    public bool IsPolynomial => Left.IsPolynomial || Right.IsPolynomial;

    public int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);

    protected abstract string GetOperator();

    public override string ToString()
    {
        return $"({Left} {GetOperator()} {Right})";
    }
}

public class Variable : IExpr
{
    private readonly string name;

    public Variable(string name)
    {
        this.name = name;
    }

    public IEnumerable<string> Variables => new[] { name };

    public bool IsConstant => false;

    public bool IsPolynomial => true;

    public int PolynomialDegree => 1;

    public double Compute(IReadOnlyDictionary<string, double> variableValues) => variableValues[name];

    public override string ToString() => name;

    // Перегрузка операторов
    public static BinaryOperation operator +(Variable a, IExpr b) => new Addition(a, b);
    public static BinaryOperation operator -(Variable a, IExpr b) => new Subtraction(a, b);
    public static BinaryOperation operator *(Variable a, IExpr b) => new Multiplication(a, b);
    public static BinaryOperation operator /(Variable a, IExpr b) => new Division(a, b);

    public static BinaryOperation operator +(Variable a, double b) => new Addition(a, new Constant(b));
    public static BinaryOperation operator -(Variable a, double b) => new Subtraction(a, new Constant(b));
    public static BinaryOperation operator *(Variable a, double b) => new Multiplication(a, new Constant(b));
    public static BinaryOperation operator /(Variable a, double b) => new Division(a, new Constant(b));
}

public class Constant : IExpr
{
    private readonly double value;

    public Constant(double value)
    {
        this.value = value;
    }

    public IEnumerable<string> Variables => Array.Empty<string>();

    public bool IsConstant => true;

    public bool IsPolynomial => true;

    public int PolynomialDegree => 0;

    public double Compute(IReadOnlyDictionary<string, double> variableValues) => value;

    public override string ToString() => value.ToString();

    // Перегрузка операторов
    public static BinaryOperation operator +(Constant a, IExpr b) => new Addition(a, b);
    public static BinaryOperation operator -(Constant a, IExpr b) => new Subtraction(a, b);
    public static BinaryOperation operator *(Constant a, IExpr b) => new Multiplication(a, b);
    public static BinaryOperation operator /(Constant a, IExpr b) => new Division(a, b);

    public static BinaryOperation operator +(IExpr a, Constant b) => new Addition(a, b);
}

public class Addition : BinaryOperation
{
    public Addition(IExpr left, IExpr right) : base(left, right) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Left.Compute(variableValues) + Right.Compute(variableValues);

    protected override string GetOperator() => "+";
}

public class Subtraction : BinaryOperation
{
    public Subtraction(IExpr left, IExpr right) : base(left, right) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Left.Compute(variableValues) - Right.Compute(variableValues);

    protected override string GetOperator() => "-";
}

public class Multiplication : BinaryOperation
{
    public Multiplication(IExpr left, IExpr right) : base(left, right) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Left.Compute(variableValues) * Right.Compute(variableValues);

    protected override string GetOperator() => "*";
}

public class Division : BinaryOperation
{
    public Division(IExpr left, IExpr right) : base(left, right) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        var rightValue = Right.Compute(variableValues);
        if (rightValue == 0)
            throw new DivideByZeroException("Division by zero is not allowed.");

        return Left.Compute(variableValues) / rightValue;
    }

    protected override string GetOperator() => "/";
}

// Пример использования
class Program
{
    static void Main()
    {
        var x = new Variable("x");
        var y = new Variable("y");

        // Теперь мы можем использовать более естественный синтаксис
        var expr1 = (x - 4) * (y * y) / 5;

        Console.WriteLine(expr1.ToString()); // ((x - 4) * (3 * x + y * y)) / 5
        Console.WriteLine(string.Join(", ", expr1.Variables)); // ["x", "y"]
        Console.WriteLine(expr1.IsConstant); // false
        Console.WriteLine(expr1.IsPolynomial); // true
        Console.WriteLine(expr1.PolynomialDegree); // 2
        Console.WriteLine(expr1.Compute(new Dictionary<string, double> { ["x"] = 1, ["y"] = 2 })); // -0.6
    }
}
