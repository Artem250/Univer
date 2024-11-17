using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection.Emit;
using System.Reflection.Metadata;
using System.Xml.Schema;

public interface IExpr
{
    IEnumerable<string> Variables { get; }
    bool IsConstant { get; }
    bool IsPolynomial { get; }
    int PolynomialDegree { get; }
    double Compute(IReadOnlyDictionary<string, double> variableValues);
}

public abstract class Expr : IExpr
{
    public abstract IEnumerable<string> Variables { get; }
    public abstract bool IsConstant { get; }
    public abstract bool IsPolynomial { get; }
    public abstract int PolynomialDegree { get; }
    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public abstract Expr Derivate(Variable x);// для доп задания дифференцирования

    public static Addition operator +(Expr left, Expr right) => new(left, right);
    public static Subtraction operator -(Expr left, Expr right) => new Subtraction(left, right);
    public static Negative operator -(Expr operand) => new Negative(operand);
    public static Multiplication operator *(Expr left, Expr right) => new Multiplication(left, right);
    public static Division operator /(Expr left, Expr right) => new Division(left, right);


    public static implicit operator Expr(int value) => new Constant(value);

    public static implicit operator Expr(double value) => new Constant(value);

}

public abstract class UnaryOperation : Expr
{
    protected Expr Operand;

    protected UnaryOperation(Expr operand)
    {
        Operand = operand;
    }

    public override abstract IEnumerable<string> Variables { get; }
    public override abstract bool IsConstant { get; }
    public override abstract bool IsPolynomial { get; }
    public override abstract int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public override string ToString() => $"{GetType().Name}({Operand})";
}

public abstract class BinaryOperation : Expr
{
    protected Expr Left, Right;

    protected BinaryOperation(Expr left, Expr right)
    {
        Left = left;
        Right = right;
    }

    public override abstract IEnumerable<string> Variables { get; }
    public override abstract bool IsConstant { get; }
    public override abstract bool IsPolynomial { get; }
    public override abstract int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
}

public abstract class Function : Expr
{
    protected Expr Operand;
    protected Function(Expr operand)
    {
        Operand = operand;
    }
    public override abstract IEnumerable<string> Variables { get; }
    public override abstract bool IsConstant { get; }
    public override abstract bool IsPolynomial { get; }
    public override abstract int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public override abstract string ToString();
}

public class Variable : Expr
{
    public string Name { get; }

    public Variable(string name)
    {
        Name = name;
    }

    public override IEnumerable<string> Variables => new[] { Name };
    public override bool IsConstant => false;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        variableValues.TryGetValue(Name, out var value) ? value : throw new ArgumentException($"Переменная {Name} не определена.");
    public override Expr Derivate(Variable x) => x == this ? new Constant(1) : new Constant(0);
    public override string ToString() => Name;
}

public class Constant : Expr
{
    public double Value { get; }

    public Constant(double value)
    {
        Value = value;
    }

    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
    public override bool IsConstant => true;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 0;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Value;
    public override Expr Derivate(Variable x) => new Constant(0);
    public override string ToString() => Value.ToString();
}

public class Addition : BinaryOperation
{
    public Addition(Expr left, Expr right) : base(left, right) { }

    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? Math.Max(Left.PolynomialDegree, Right.PolynomialDegree) : -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Left.Compute(variableValues) + Right.Compute(variableValues);
    public override Expr Derivate(Variable x) => Left.Derivate(x) + Right.Derivate(x);
    public override string ToString() => $"({Left} + {Right})";
}

public class Subtraction : BinaryOperation
{
    public Subtraction(Expr left, Expr right) : base(left, right) { }

    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? Math.Max(Left.PolynomialDegree, Right.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Left.Compute(variableValues) - Right.Compute(variableValues);
    public override Expr Derivate(Variable x) => Left.Derivate(x) - Right.Derivate(x);

    public override string ToString() => $"({Left} - {Right})";
}

public class Multiplication : BinaryOperation
{
    public Multiplication(Expr left, Expr right) : base(left, right) { }

    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? (Left.PolynomialDegree + Right.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Left.Compute(variableValues) * Right.Compute(variableValues);
    public override Expr Derivate(Variable x) => Left.Derivate(x) * Right + Left * Right.Derivate(x);

    public override string ToString() => $"({Left}*{Right})";
}

public class Division : BinaryOperation
{
    public Division(Expr left, Expr right) : base(left, right) { }

    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? (Left.PolynomialDegree - Right.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Left.Compute(variableValues) / Right.Compute(variableValues);

    public override Expr Derivate(Variable x) => (Left.Derivate(x) * Right - Right * Left.Derivate(x)) / (Right * Right);

    public override string ToString() => $"({Left}/{Right})";
}

public class SinFun : Function
{
    public SinFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false; // Синус не является многочленом
    public override int PolynomialDegree => -1; // Не определено для тригонометрических функций
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Sin(Operand.Compute(variableValues));

    public override Expr Derivate(Variable x) => new CosFun(Operand) * Operand.Derivate(x);

    public override string ToString() => $"sin({Operand})";
}

public class CosFun : Function
{
    public CosFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для тригонометрических функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Cos(Operand.Compute(variableValues));

    public override Expr Derivate(Variable x) => new SinFun(Operand) * Operand.Derivate(x) * (-1);

    public override string ToString() => $"cos({Operand})";
}

public class ArcSinFun : Function
{
    public ArcSinFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Asin(Operand.Compute(variableValues));
    public override Expr Derivate(Variable x) => Operand.Derivate(x) / (new SqrtFun(1 - Operand * Operand));
    public override string ToString() => $"arcsin({Operand})";
}

public class ArcCosFun : Function
{
    public ArcCosFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Acos(Operand.Compute(variableValues));
    public override Expr Derivate(Variable x) => (-1) * Operand.Derivate(x) / (new SqrtFun(1 - Operand * Operand));

    public override string ToString() => $"arccos({Operand})";
}

public class ArcTanFun : Function
{
    public ArcTanFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Atan(Operand.Compute(variableValues));

    public override Expr Derivate(Variable x) => Operand.Derivate(x) / (1 + Operand * Operand);

    public override string ToString() => $"arctan({Operand})";
}

public class ArcCotFun : Function
{
    public ArcCotFun(Expr operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.PI / 2 - Math.Atan(Operand.Compute(variableValues));
    public override Expr Derivate(Variable x) => (-1) * Operand.Derivate(x) / (1 + Operand * Operand);
    public override string ToString() => $"arccot({Operand})";
}

public class SqrtFun : Function
{
    public SqrtFun(Expr operand) : base(operand) { }

    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false; // Корень не является полиномом
    public override int PolynomialDegree => -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Sqrt(Operand.Compute(variableValues));
    public override Expr Derivate(Variable x) => (0.5) * Operand.Derivate(x) / new SqrtFun(Operand);
    public override string ToString() => $"Sqrt({Operand})";

}

public class Negative : UnaryOperation
{
    public Negative(Expr operand) : base(operand) { }

    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => Operand.IsPolynomial;
    public override int PolynomialDegree => Operand.PolynomialDegree;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => (-1) * Operand.Compute(variableValues);
    public override Expr Derivate(Variable x) => -1 * Operand;
    public override string ToString() => $"(-{Operand})";
}

static internal class IExprInfo
{
    public static void PrintAllInfo(Expr expr)
    {
        Console.WriteLine($"Вся информация о выражении {expr}");
        Console.WriteLine($"Список переменных в выражении: {string.Join(", ", expr.Variables)}");
        Console.WriteLine(expr.IsConstant ? "Выражение является константой" : "Выражение не является константой");
        Console.WriteLine(expr.IsPolynomial ? $"Выражение является полиномом\nСтепень полинома {expr.PolynomialDegree}" : "Выражение не является полиномом");
        foreach (var variable_ in expr.Variables)
            Console.WriteLine($"Производная по переменной {variable_}: {expr.Derivate(new Variable(variable_))}");
    }
}

public class Program
{
    public static SqrtFun Sqrt(Expr operand) => new SqrtFun(operand);
    public static ArcSinFun ArcSin(Expr operand) => new ArcSinFun(operand);
    public static ArcCosFun ArcCos(Expr operand) => new ArcCosFun(operand);
    public static ArcTanFun ArcTan(Expr operand) => new ArcTanFun(operand);
    public static ArcCotFun ArcCot(Expr operand) => new ArcCotFun(operand);

    public static void Main()
    {
        var input1 = double.Parse(Console.ReadLine());
        var input2 = double.Parse(Console.ReadLine());
        var E = new Constant(Math.E);
        var PI = new Constant(Math.PI);
        var x = new Variable("x");
        var y = new Variable("y");
        var c = new Constant(3);

        var d = -x;

        var expr3 = x * c;
        var expr1 = (x - 4) * (3 * x + y * y) / 5;
        var expr2 = (5 - 3 * c) * Sqrt(16 + c * c);
        var expr4 = Sqrt(2 * x + 4);
        double result = expr1.Compute(new Dictionary<string, double> { { "x", input1 }, { "y", input2 } });

        Console.WriteLine(d);

        IExprInfo.PrintAllInfo(expr1);
        expr1.Compute(new Dictionary<string, double>
        { ["x"] = 1, ["y"] = 2 });

    }
}

