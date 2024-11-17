using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Dynamic;
using System.Reflection.Emit;
using System.Reflection.Metadata;
using System.Runtime.CompilerServices;
using System.Security.AccessControl;

static LogFun Log(Expr left, Expr right) => new LogFun(left, right);
static PowFun Pow(Expr left, Expr right) => new PowFun(left, right);
static SqrtFun Sqrt(Expr operand) => new SqrtFun(operand);

var x = new Variable("x");
var y = new Variable("y");
var z = new Variable("z");
var w = new Variable("w");
var c = new Constant(3);
//var expr1 = (x - 4.0) * (3 * x + y * y) / 5;

//var expr1 = (x - 4) * (3 * x + y * y) / 5;
//var expr1 = Pow(x, 2) / (x * x * x);
//var expr1 = Pow(x, 2) + Log(x, 8);
//var expr1 = Pow(x, 3.2);
//var expr1 = Log(2, 8);
var expr1 = x - x;

//var expr1 = (5 - 3 * c) * Sqrt(16 + c * c);


//var expr2 = Log(2, 8.0);

//var expr3 = Pow(x, 4) / Pow(y, 2); //+ (2 * Pow(y, 2)) - (5 * Pow(x, 2)) - Pow(y, 2);
//var expr3 = Pow(2, x) + x;
//Console.WriteLine(Math.Pow(2, 2));
Console.WriteLine(expr1.ToString());
Console.WriteLine($"[{string.Join(", ", expr1.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
Console.WriteLine(expr1.IsConstant);
Console.WriteLine(expr1.IsPolynomial);
Console.WriteLine("PolynomialDegree: " + expr1.PolynomialDegree);
Console.WriteLine(expr1.Compute(new Dictionary<string, double>
{ ["x"] = 2, ["y"] = 2 }));

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
    public abstract override string ToString();
    //public abstract Expr Derivate(Variable x);// для доп задания дифференцирования


    public static Add operator +(Expr left, Expr right) => new(left, right);
    public static Sub operator -(Expr left, Expr right) => new(left, right);
    public static Negative operator -(Expr operand) => new Negative(operand);
    public static Mult operator *(Expr left, Expr right) => new(left, right);
    public static Div operator /(Expr left, Expr right) => new(left, right);
    public static implicit operator Expr(int value) => new Constant(value);
    public static implicit operator Expr(double value) => new Constant(value);
}

//у Антона абстрактные классы написаны через get. Тут хз как лучше
public abstract class UnaryOperation : Expr
{
    protected Expr Operand { get; }
    protected UnaryOperation(Expr operand) { Operand = operand; }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant { get; }
    public override bool IsPolynomial { get; }
    public override int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public abstract override string ToString();
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
    public abstract override string ToString();
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

//public class Constant : Expr
//{
//    public double Value { get; }
//    public Constant(double value) { Value = value; }
//    public Constant(int value) { Value = value; }
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Value;
//    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
//    public override bool IsConstant => true;
//    public override bool IsPolynomial => true;
//    public override int PolynomialDegree => 0;
//    public override string ToString() => Value.ToString();
//}

public class Constant : Expr
{
    public object Value { get; }  // Изменено на object

    public Constant(double value) { Value = value; }
    public Constant(int value) { Value = value; }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Value is double dValue ? dValue : (int)Value;

    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
    public override bool IsConstant => true;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 0;

    public override string ToString() => Value.ToString();
}


public class Variable : Expr
{
    public string name { get; }
    public Variable(string name) { this.name = name; }
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => variableValues[name];
    public override IEnumerable<string> Variables => new List<string> { name };
    public override bool IsConstant => false;
    public override bool IsPolynomial => true;
    public override int PolynomialDegree => 1;
    public override string ToString() => name;
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
}
public class Mult : BinaryOperation
{
    public Mult(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => true; //??
    public override int PolynomialDegree => (Left.PolynomialDegree + Right.PolynomialDegree);
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) * Right.Compute(variableValues);
    }
    public override string ToString() => $"({Left} * {Right})";
}
//public class Div : BinaryOperation
//{
//    public Div(IExpr left, IExpr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
//    public override int PolynomialDegree => Left.PolynomialDegree - Right.PolynomialDegree;
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Left.Compute(variableValues) / Right.Compute(variableValues);
//    }
//    public override string ToString() => $"({Left} / {Right})";
//}

public class Div : BinaryOperation
{
    public Div(Expr left, Expr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;

    //public override bool IsPolynomial =>
    //Left.IsPolynomial && Right.IsPolynomial && !Left.Variables.Except(Right.Variables).Any() && !Right.Variables.Except(Left.Variables).Any();

    public override bool IsPolynomial => /*PolynomialDegree >= 0 &&*/ Left.IsPolynomial && Right.IsConstant;
    //Left.IsPolynomial && Right.IsPolynomial && (Right is Constant || Left.Variables.SequenceEqual(Right.Variables)) &&
    //    Left.PolynomialDegree == Right.PolynomialDegree;

    //(Right is Constant || (!Left.Variables.Except(Right.Variables).Any() && !Right.Variables.Except(Left.Variables).Any()));


    public override int PolynomialDegree => IsPolynomial ? Left.PolynomialDegree - Right.PolynomialDegree : 0;
    //IsPolynomial ? Left.PolynomialDegree - Right.PolynomialDegree : 0;
    //Left.Variables.SequenceEqual(Right.Variables) ? Left.PolynomialDegree - Right.PolynomialDegree : 0; //последний вариант

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) / Right.Compute(variableValues);
    }

    public override string ToString() => $"({Left} / {Right})";
}


//public class PowFun : Function
//{
//    public PowFun(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynom => Left.IsPolynom && Right.IsConstant;
//    public override int PolynomialDegree => 1; //??
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Math.Pow(Left.Compute(variableValues), Right.Compute(variableValues));
//    }
//    public override string ToString() => $"({Left} ^ {Right})";
//}

public class PowFun : Function
{
    //public PowFun(Expr left, int right) : base(left, new Constant(right)) { }

    public PowFun(Expr left, Expr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Right is Constant rightConst && rightConst.Value is int;
    //public override int PolynomialDegree => Left.IsConstant ? 0 : (Right is Constant rightConst ? (int)rightConst.Value : 0);
    public override int PolynomialDegree =>
        Left.IsConstant ? 0 : (Right is Constant rightConst && rightConst.Value is int val ? val : 0);


    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Math.Pow(Left.Compute(variableValues), Right.Compute(variableValues));
    }

    public override string ToString() => $"({Left} ^ {Right})";
}

public class LogFun : Function
{
    public LogFun(Expr left, Expr right) : base(left, right) { }
    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynomial => Left.IsConstant && Right.IsConstant;
    public override int PolynomialDegree => 0;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Math.Log(Right.Compute(variableValues), Left.Compute(variableValues));
    }
    public override string ToString() => $"Log{Left}({Right})";

}

public class SqrtFun : UnaryOperation
{
    public SqrtFun(Expr operand) : base(operand) { }
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => Operand.IsConstant;
    public override int PolynomialDegree => 0;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Math.Sqrt(Operand.Compute(variableValues));
    }
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
    public override string ToString() => $"(-{Operand})";
}

