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

public abstract class Extensions : IExpr
{
    public abstract IEnumerable<string> Variables { get; }
    public abstract bool IsConstant { get; }
    public abstract bool IsPolynomial { get; }
    public abstract int PolynomialDegree { get; }
    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
    public abstract Extensions Derivate(Variable x);// для доп задания дифференцирования

    public static Addition operator +(Extensions left, Extensions right) => new Addition(left, right);
    public static Subtraction operator -(Extensions left, Extensions right) => new Subtraction(left, right);
    public static Negative operator -(Extensions operand) => new Negative(operand);
    public static Multiplication operator *(Extensions left, Extensions right) => new Multiplication(left, right);
    public static Division operator /(Extensions left, Extensions right) => new Division(left, right);


    public static implicit operator Extensions(int value) => new Constant((double)value);

    public static implicit operator Extensions(double value) => new Constant(value);

}

public abstract class UnaryOperation : Extensions
{
    protected Extensions Operand;

    protected UnaryOperation(Extensions operand)
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

public abstract class BinaryOperation : Extensions
{
    protected Extensions LeftOperand, RightOperand;

    protected BinaryOperation(Extensions left, Extensions right)
    {
        LeftOperand = left;
        RightOperand = right;
    }

    public override abstract IEnumerable<string> Variables { get; }
    public override abstract bool IsConstant { get; }
    public override abstract bool IsPolynomial { get; }
    public override abstract int PolynomialDegree { get; }
    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
}

public abstract class Function : Extensions
{
    protected Extensions Operand;
    protected Function(Extensions operand)
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

public class Variable : Extensions
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
    public override Extensions Derivate(Variable x) => x == this ? new Constant(1) : new Constant(0);
    public override string ToString() => Name;
}

public class Constant : Extensions
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
    public override Extensions Derivate(Variable x) => new Constant(0);
    public override string ToString() => Value.ToString();
}

public class Addition : BinaryOperation
{
    public Addition(Extensions left, Extensions right) : base(left, right) { }

    public override IEnumerable<string> Variables => LeftOperand.Variables.Union(RightOperand.Variables);
    public override bool IsConstant => LeftOperand.IsConstant && RightOperand.IsConstant;
    public override bool IsPolynomial => LeftOperand.IsPolynomial && RightOperand.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? Math.Max(LeftOperand.PolynomialDegree, RightOperand.PolynomialDegree) : -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => LeftOperand.Compute(variableValues) + RightOperand.Compute(variableValues);
    public override Extensions Derivate(Variable x) => LeftOperand.Derivate(x) + RightOperand.Derivate(x);
    public override string ToString() => $"({LeftOperand} + {RightOperand})";
}

public class Subtraction : BinaryOperation
{
    public Subtraction(Extensions left, Extensions right) : base(left, right) { }

    public override IEnumerable<string> Variables => LeftOperand.Variables.Union(RightOperand.Variables);
    public override bool IsConstant => LeftOperand.IsConstant && RightOperand.IsConstant;
    public override bool IsPolynomial => LeftOperand.IsPolynomial && RightOperand.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? Math.Max(LeftOperand.PolynomialDegree, RightOperand.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => LeftOperand.Compute(variableValues) - RightOperand.Compute(variableValues);
    public override Extensions Derivate(Variable x) => LeftOperand.Derivate(x) - RightOperand.Derivate(x);

    public override string ToString() => $"({LeftOperand} - {RightOperand})";
}

public class Multiplication : BinaryOperation
{
    public Multiplication(Extensions left, Extensions right) : base(left, right) { }

    public override IEnumerable<string> Variables => LeftOperand.Variables.Union(RightOperand.Variables);
    public override bool IsConstant => LeftOperand.IsConstant && RightOperand.IsConstant;
    public override bool IsPolynomial => LeftOperand.IsPolynomial && RightOperand.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? (LeftOperand.PolynomialDegree + RightOperand.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => LeftOperand.Compute(variableValues) * RightOperand.Compute(variableValues);
    public override Extensions Derivate(Variable x) => LeftOperand.Derivate(x) * RightOperand + LeftOperand * RightOperand.Derivate(x);

    public override string ToString() => $"({LeftOperand}*{RightOperand})";
}

public class Division : BinaryOperation
{
    public Division(Extensions left, Extensions right) : base(left, right) { }

    public override IEnumerable<string> Variables => LeftOperand.Variables.Union(RightOperand.Variables);
    public override bool IsConstant => LeftOperand.IsConstant && RightOperand.IsConstant;
    public override bool IsPolynomial => LeftOperand.IsPolynomial && RightOperand.IsPolynomial;
    public override int PolynomialDegree => IsPolynomial ? (LeftOperand.PolynomialDegree - RightOperand.PolynomialDegree) : -1;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => LeftOperand.Compute(variableValues) / RightOperand.Compute(variableValues);

    public override Extensions Derivate(Variable x) => (LeftOperand.Derivate(x) * RightOperand - RightOperand * LeftOperand.Derivate(x)) / (RightOperand * RightOperand);

    public override string ToString() => $"({LeftOperand}/{RightOperand})";
}

public class SinFun : Function
{
    public SinFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false; // Синус не является многочленом
    public override int PolynomialDegree => -1; // Не определено для тригонометрических функций
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Sin(Operand.Compute(variableValues));

    public override Extensions Derivate(Variable x) => new CosFun(Operand) * Operand.Derivate(x);

    public override string ToString() => $"sin({Operand})";
}

public class CosFun : Function
{
    public CosFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для тригонометрических функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Cos(Operand.Compute(variableValues));

    public override Extensions Derivate(Variable x) => new SinFun(Operand) * Operand.Derivate(x) * (-1);

    public override string ToString() => $"cos({Operand})";
}

public class ArcSinFun : Function
{
    public ArcSinFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Asin(Operand.Compute(variableValues));
    public override Extensions Derivate(Variable x) => Operand.Derivate(x) / (new SqrtFun(1 - Operand * Operand));
    public override string ToString() => $"arcsin({Operand})";
}

public class ArcCosFun : Function
{
    public ArcCosFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Acos(Operand.Compute(variableValues));
    public override Extensions Derivate(Variable x) => (-1) * Operand.Derivate(x) / (new SqrtFun(1 - Operand * Operand));

    public override string ToString() => $"arccos({Operand})";
}

public class ArcTanFun : Function
{
    public ArcTanFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Atan(Operand.Compute(variableValues));

    public override Extensions Derivate(Variable x) => Operand.Derivate(x) / (1 + Operand * Operand);

    public override string ToString() => $"arctan({Operand})";
}

public class ArcCotFun : Function
{
    public ArcCotFun(Extensions operand) : base(operand) { }
    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false;
    public override int PolynomialDegree => -1; // Не определено для обратных тригонометрическим функций

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.PI / 2 - Math.Atan(Operand.Compute(variableValues));
    public override Extensions Derivate(Variable x) => (-1) * Operand.Derivate(x) / (1 + Operand * Operand);
    public override string ToString() => $"arccot({Operand})";
}

public class SqrtFun : Function
{
    public SqrtFun(Extensions operand) : base(operand) { }

    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => false; // Корень не является полиномом
    public override int PolynomialDegree => -1;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Math.Sqrt(Operand.Compute(variableValues));
    public override Extensions Derivate(Variable x) => (0.5) * Operand.Derivate(x) / new SqrtFun(Operand);
    public override string ToString() => $"Sqrt({Operand})";

}

public class Negative : UnaryOperation
{
    public Negative(Extensions operand) : base(operand) { }

    public override IEnumerable<string> Variables => Operand.Variables;
    public override bool IsConstant => Operand.IsConstant;
    public override bool IsPolynomial => Operand.IsPolynomial;
    public override int PolynomialDegree => Operand.PolynomialDegree;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => (-1) * Operand.Compute(variableValues);
    public override Extensions Derivate(Variable x) => -1 * Operand;
    public override string ToString() => $"(-{Operand})";
}

static internal class IExprInfo
{
    public static void PrintAllInfo(Extensions expr)
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
    public static SqrtFun Sqrt(Extensions operand) => new SqrtFun(operand);
    public static ArcSinFun ArcSin(Extensions operand) => new ArcSinFun(operand);
    public static ArcCosFun ArcCos(Extensions operand) => new ArcCosFun(operand);
    public static ArcTanFun ArcTan(Extensions operand) => new ArcTanFun(operand);
    public static ArcCotFun ArcCot(Extensions operand) => new ArcCotFun(operand);

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






















//using System.Collections.ObjectModel;
//using System.ComponentModel;
//using System.ComponentModel.DataAnnotations;
//using System.Dynamic;
////using System.Linq;
//using System.Reflection.Emit;
//using System.Reflection.Metadata;
//using System.Runtime.CompilerServices;
//using System.Security.AccessControl;
//using System.Threading.Tasks.Dataflow;

//static LogFun Log(Expr left, Expr right) => new LogFun(left, right);
//static PowFun Pow(Expr left, double right) => new PowFun(left, right);
//static SqrtFun Sqrt(Expr operand) => new SqrtFun(operand);

//var x = new Variable("x");
//var y = new Variable("y");
//var z = new Variable("z");
//var w = new Variable("w");
//var c = new Constant(3);
//var expr1 = (x - 4.0) * (3 * x + y * y) / 5;
////var expr2 = Log(2, 8.0);
//var expr2 = (5 - 3 * c) * Sqrt(16 + c * c);

//var expr3 = 2 * Pow(y, 2);// - 5 * Pow(x, 2) - 2 * Pow(y, 2);
////var expr3 = Pow(2, x) + x;
////Console.WriteLine(Math.Pow(2, 2));
//Console.WriteLine(expr1.ToString());
//Console.WriteLine($"[{string.Join(", ", expr1.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr1.IsConstant);
//Console.WriteLine(expr1.IsPolynomial);
//Console.WriteLine(expr1.PolynomialDegree);
//Console.WriteLine(expr1.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));

//Console.WriteLine("-----------------------------------");
//Console.WriteLine(expr2.ToString());
//Console.WriteLine($"[{string.Join(", ", expr2.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr2.IsConstant);
//Console.WriteLine(expr2.IsPolynomial);
//Console.WriteLine(expr2.PolynomialDegree);
//Console.WriteLine(expr2.Compute(new Dictionary<string, double>
//{ ["x"] = 2, ["y"] = 2 }));

//Console.WriteLine("-----------------------------------");

//Console.WriteLine(expr3.ToString());
//Console.WriteLine($"[{string.Join(", ", expr3.Variables.Select(v => $"\"{v}\""))}]"); // Вывод: ["x", "y"]
//Console.WriteLine(expr3.IsConstant);
//Console.WriteLine(expr3.IsPolynomial);
//Console.WriteLine(expr3.PolynomialDegree);

//Console.WriteLine(expr3.Compute(new Dictionary<string, double>
//{ ["x"] = 1, ["y"] = 2 }));

//public interface IExpr
//{
//    double Compute(IReadOnlyDictionary<string, double> variableValues);
//    IEnumerable<string> Variables { get; }
//    bool IsConstant { get; }
//    bool IsPolynomial { get; }
//    int PolynomialDegree { get; }
//}

//public abstract class Expr : IExpr
//{
//    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
//    public abstract IEnumerable<string> Variables { get; }
//    public abstract bool IsConstant { get; }
//    public abstract bool IsPolynomial { get; }
//    public abstract int PolynomialDegree { get; }
//    public abstract override string ToString();

//    public static Negative operator -(Expr operand) => new Negative(operand);
//    public static Add operator +(Expr left, Expr right) => new(left, right);
//    public static Sub operator -(Expr left, Expr right) => new(left, right);
//    public static Mult operator *(Expr left, Expr right) => new(left, right);
//    public static Div operator /(Expr left, Expr right) => new(left, right);
//    //public static implicit operator Expr(int value) => new Constant(value);
//    public static implicit operator Expr(double value) => new Constant(value);
//}

////у Антона абстрактные классы написаны через get. Тут хз как лучше
//public abstract class UnaryOperation : Expr
//{
//    protected IExpr Operand { get; }
//    protected UnaryOperation(IExpr operand) { Operand = operand; }
//    public override IEnumerable<string> Variables { get; }
//    public override bool IsConstant { get; }
//    public override bool IsPolynomial { get; }
//    public override int PolynomialDegree { get; }
//    public override abstract double Compute(IReadOnlyDictionary<string, double> variableValues);
//    public abstract override string ToString();
//}

//public abstract class BinaryOperation : Expr
//{
//    protected Expr Left { get; }
//    protected Expr Right { get; }
//    protected BinaryOperation(Expr left, Expr right) { Left = left; Right = right; }
//    public override IEnumerable<string> Variables { get; }//=> Left.Variables.Union(Right.Variables);
//    public override bool IsConstant { get; }
//    public override bool IsPolynomial { get; }
//    public override int PolynomialDegree { get; }
//    public abstract override string ToString();
//}

//public abstract class Function : Expr
//{
//    protected Expr Left { get; }
//    protected Expr Right { get; }
//    protected Function(Expr left, Expr right) { Left = left; Right = right; }
//    public override IEnumerable<string> Variables => Left.Variables.Union(Right.Variables);
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
//    public override int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);
//}


//public class Constant : Expr
//{
//    public double Value { get; }
//    public Constant(double value) { Value = value; }
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => Value;
//    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
//    public override bool IsConstant => true;
//    public override bool IsPolynomial => true;
//    public override int PolynomialDegree => 0;
//    public override string ToString() => Value.ToString();
//}

//public class Variable : Expr
//{
//    public string name { get; }
//    public Variable(string name) { this.name = name; }
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => variableValues[name];
//    public override IEnumerable<string> Variables => new List<string> { name };
//    public override bool IsConstant => false;
//    public override bool IsPolynomial => true;
//    public override int PolynomialDegree => 1;
//    public override string ToString() => name;
//}

//public class Negative : UnaryOperation
//{
//    public Negative(Expr operand) : base(operand) { }

//    public override IEnumerable<string> Variables => Operand.Variables;
//    public override bool IsConstant => Operand.IsConstant;
//    public override bool IsPolynomial => Operand.IsPolynomial;
//    public override int PolynomialDegree => Operand.PolynomialDegree;

//    public override double Compute(IReadOnlyDictionary<string, double> variableValues) => (-1) * Operand.Compute(variableValues);
//    public override string ToString() => $"(-{Operand})";
//}

//public class Add : BinaryOperation
//{
//    public Add(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => true;
//    public override int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Left.Compute(variableValues) + Right.Compute(variableValues);
//    }
//    public override string ToString() => $"({Left} + {Right})";
//}
//public class Sub : BinaryOperation
//{
//    public Sub(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => true;
//    public override int PolynomialDegree => Math.Max(Left.PolynomialDegree, Right.PolynomialDegree);
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Left.Compute(variableValues) - Right.Compute(variableValues);
//    }
//    public override string ToString() => $"({Left} - {Right})";
//}
//public class Mult : BinaryOperation
//{
//    public Mult(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial; //??
//    public override int PolynomialDegree => (Left.PolynomialDegree + Right.PolynomialDegree);
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Left.Compute(variableValues) * Right.Compute(variableValues);
//    }
//    public override string ToString() => $"({Left} * {Right})";
//}
//public class Div : BinaryOperation
//{
//    public Div(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => Left.IsPolynomial && Right.IsPolynomial;
//    public override int PolynomialDegree => Left.PolynomialDegree - Right.PolynomialDegree;
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Left.Compute(variableValues) / Right.Compute(variableValues);
//    }
//    public override string ToString() => $"({Left} / {Right})";
//}

////public class PowFun : Function
////{

////    //public static implicit operator LogFun(Expr expr ,int value) => new Constant(value);

////    public PowFun(Expr left, double right) : base(left, right) { }
////    //private int right => int.Parse(string.Join(", ", Right.Variables.Where(Value);

////    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
////    public override bool IsPolynomial => Right.IsConstant;
////    public override int PolynomialDegree => Left.IsConstant ? 0 : ;
////    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
////    {
////        return Math.Pow(Left.Compute(variableValues), Right.Compute(variableValues));
////    }
////    public override string ToString() => $"({Left} ^ {Right})";
////}

//public class PowFun : Function
//{
//    public PowFun(Expr left, double right) : base(left, new Constant(right)) { }

//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => Right.IsConstant;
//    public override int PolynomialDegree => Left.IsConstant ? 0 : (Right is Constant rightConst ? (int)rightConst.Value : 0);

//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Math.Pow(Left.Compute(variableValues), Right.Compute(variableValues));
//    }

//    public override string ToString() => $"({Left} ^ {Right})";
//}


//public class LogFun : Function
//{
//    public LogFun(Expr left, Expr right) : base(left, right) { }
//    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
//    public override bool IsPolynomial => IsConstant;
//    public override int PolynomialDegree => 0; //??
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Math.Log(Right.Compute(variableValues), Left.Compute(variableValues));
//    }
//    public override string ToString() => $"Log{Left}({Right})";

//}
//public class SqrtFun : UnaryOperation
//{
//    public SqrtFun(Expr operand) : base(operand) { }
//    public override bool IsConstant => Operand.IsConstant;
//    public override bool IsPolynomial => false;
//    public override int PolynomialDegree => 0; //Тут хз на самом деле, у Антона стоит -1, однако в таком случае ответы не будут сходиться
//    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
//    {
//        return Math.Sqrt(Operand.Compute(variableValues));
//    }
//    public override string ToString() => $"Sqrt({Operand})";

//}
