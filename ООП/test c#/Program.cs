var a = new Variable("a");
var b = new Variable("b");
//var expr0 = new Mult(new Add(a, b), new SinhFunc(new Div(a, new Constant(2))));
var expr = (a + b) * new SinhFunc(a / 2);
Console.WriteLine(expr);
Console.WriteLine(expr.Compute(new Dictionary<string, double> { ["a"] = 5, ["b"] = 3 }));

public interface IExpr
{
    IEnumerable<string> Variables { get; }
    bool IsConstant { get; }
    bool IsPolynom { get; }
    double Compute(IReadOnlyDictionary<string, double> variableValues);
}

public abstract class Expr : IExpr
{
    public abstract IEnumerable<string> Variables { get; }
    public abstract bool IsConstant { get; }
    public abstract bool IsPolynom { get; }
    public abstract override string ToString();
    public abstract double Compute(IReadOnlyDictionary<string, double> variableValues);

    public static Add operator +(Expr left, Expr right) => new(left, right);
    public static Sub operator -(Expr left, Expr right) => new(left, right);
    public static Mult operator *(Expr left, Expr right) => new(left, right);
    public static Div operator /(Expr left, Expr right) => new(left, right);

    public static Add operator +(Expr left, double right) => new(left, new Constant(right));
    public static Sub operator -(Expr left, double right) => new(left, new Constant(right));
    public static Mult operator *(Expr left, double right) => new(left, new Constant(right));
    public static Div operator /(Expr left, double right) => new(left, new Constant(right));

    public static Add operator +(double left, Expr right) => new(new Constant(left), right);
    public static Sub operator -(double left, Expr right) => new(new Constant(left), right);
    public static Mult operator *(double left, Expr right) => new(new Constant(left), right);
    public static Div operator /(double left, Expr right) => new(new Constant(left), right);
}


// Операции (унарная, бинарная), функция

public abstract class UnaryOperation : Expr
{
    protected IExpr Parameter;

    protected UnaryOperation(IExpr parameter)
    {
        Parameter = parameter;
    }
    public override IEnumerable<string> Variables => Parameter.Variables;

    public abstract override string ToString();
}

public abstract class BinaryOperation : Expr
{
    protected IExpr Left;
    protected IExpr Right;

    protected BinaryOperation(IExpr left, IExpr right)
    {
        Left = left;
        Right = right;
    }

    public override IEnumerable<string> Variables => Left.Variables.Concat(Right.Variables);
    public abstract override string ToString();
}

public abstract class Function : UnaryOperation
{
    protected Function(IExpr operand) : base(operand) { }
}

// Переменная, константа

public class Variable : Expr
{
    public string Name { get; }

    public Variable(string name)
    {
        Name = name;
    }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return variableValues[Name];
    }

    public override IEnumerable<string> Variables => new List<string> { Name };
    public override bool IsConstant => false;
    public override bool IsPolynom => true;

    public override string ToString()
    {
        return Name;
    }
}

public class Constant : Expr
{
    public double Value { get; }

    public Constant(double value)
    {
        Value = value;
    }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Value;
    }

    public override IEnumerable<string> Variables => Enumerable.Empty<string>();
    public override bool IsConstant => true;
    public override bool IsPolynom => true;

    public override string ToString()
    {
        return Value.ToString();
    }
}

// Операции (+ - * /)

public class Add : BinaryOperation
{
    public Add(IExpr left, IExpr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynom => true;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) + Right.Compute(variableValues);
    }

    public override string ToString()
    {
        return $"({Left} + {Right})";
    }
}

public class Sub : BinaryOperation
{
    public Sub(IExpr left, IExpr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynom => true;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) - Right.Compute(variableValues);
    }

    public override string ToString()
    {
        return $"({Left} + {Right})";
    }
}

public class Mult : BinaryOperation
{
    public Mult(IExpr left, IExpr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynom => Left.IsPolynom && Right.IsPolynom;

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) * Right.Compute(variableValues);
    }

    public override string ToString()
    {
        return $"({Left} * {Right})";
    }
}

public class Div : BinaryOperation
{
    public Div(IExpr left, IExpr right) : base(left, right) { }

    public override bool IsConstant => Left.IsConstant && Right.IsConstant;
    public override bool IsPolynom => Left.IsPolynom && Right.IsPolynom;
    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        return Left.Compute(variableValues) / Right.Compute(variableValues);
    }

    public override string ToString()
    {
        return $"({Left} / {Right})";
    }
}

public class SinhFunc : Function
{
    public SinhFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Math.Sinh(Parameter.Compute(variableValues));

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"sinh({Parameter})";
}

public class CoshFunc : Function
{
    public CoshFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Math.Cosh(Parameter.Compute(variableValues));

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"cosh({Parameter})";
}

public class TanhFunc : Function
{
    public TanhFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues) =>
        Math.Tanh(Parameter.Compute(variableValues));

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"tanh({Parameter})";
}

public class CothFunc : Function
{
    public CothFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        double tanhValue = Math.Tanh(Parameter.Compute(variableValues));
        if (tanhValue == 0) throw new InvalidOperationException("Division by zero in coth");
        return 1.0 / tanhValue;
    }

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"coth({Parameter})";
}

public class SechFunc : Function
{
    public SechFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        double coshValue = Math.Cosh(Parameter.Compute(variableValues));
        if (coshValue == 0) throw new InvalidOperationException("Division by zero in sech");
        return 1.0 / coshValue;
    }

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"sech({Parameter})";
}

public class CschFunc : Function
{
    public CschFunc(IExpr operand) : base(operand) { }

    public override double Compute(IReadOnlyDictionary<string, double> variableValues)
    {
        double sinhValue = Math.Sinh(Parameter.Compute(variableValues));
        if (sinhValue == 0) throw new InvalidOperationException("Division by zero in csch");
        return 1.0 / sinhValue;
    }

    public override bool IsConstant => Parameter.IsConstant;

    public override bool IsPolynom => false;

    public override string ToString() => $"csch({Parameter})";
}













/*using System.Collections.Generic;





















IEnumerable<IPerson> people = [

    new Person(){Name="123"},
    new Person(){Name="13"},
];

//PrintNames(CreatePersons(7));
ICollection<Person> x;
IList<IPerson> y; //Add, Remove, индексатор, [i] 
ISet<IPerson> z; //интерфейс множества, позволяет проверять есть ли объект в множестве
//две реализации
HashSet<IPerson> s; //unordered map, быстрее чем сортед сет
SortedSet<IPerson> s2; // двоичное дерево, обычный set в c++

Dictionary<string, IPerson> s3; //два параметра, 
IDictionary<string, IPerson> s4;
SortedDictionary<string, IPerson> s5;
//Person[,] persons = new Person[100, 120];
var persons = CreatePersons(4);
var persons2 = CreatePersons(3);

var res =
    from p in persons
    let i = int.Parse(p.Name)
    where i > 2
    from q in persons2
    select (p.Name, q);

foreach (var p in res)
    Console.WriteLine(p);



//List<T> //+ IList
// Stack<T>
//Queue<T>
//ISet<T>
//все коллекциии в шарпе реализуют интерфейс перечисления IEnumerable, по которму можно бегать в цикле
void PrintNames(IEnumerable<IPerson> people)
{
    foreach (var person in people)
        Console.WriteLine(person.Name);
}

IEnumerable<IPerson> CreatePersons(int n)
{
    for (int i = 0; i < n; i++)
    {
        if (i == 3) yield break;//заканчивает последовательность
        yield return new Person() { Name = i.ToString() };
    }
}
//yield return возвращает следующий элемент последовательности

public interface IPerson
{
    string Name { get; }
}

public class Person : IPerson
{
    public string Name { get; set; }
}




/*A a = new A() { X = 10 };
A b = new B() { Y = 25, X = 12 };

a.Print();
b.Print();

public class A
{
    public int X { get; set; }
    public virtual void Print()
    {
        Console.WriteLine($"X={X}");
    }

}

public class B : A
{
    public int Y { get; set; }
    public override void Print()
    {
        base.Print();
        Console.WriteLine($"Y={Y}");
    }
}
//все наследники могут переопределять виртуальные функции с помощью override 
//new не переопределяет
*/










/*
public class MyClass
{
    required public int X { get; set; }
    private int y;
    protected int z;

    public delegate void Fun(int x, int y, int z);
    public event Fun MyFun;
    static public void RunEvent(MyClass a)
    {
        a.MyFun(x, y, z);
    }
}

static class TestExtension
{
    public static void Extension(this MyClass)
}*/