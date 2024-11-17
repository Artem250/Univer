Console.WriteLine("Hello, World!");

// 1. ввод слау
// 2. решение
// 3. вывод результата

public interface ISlaeSolver
{
    double Eps { get; init; }
    int MaxIter { get; init; }
    double[] Solve(ISLAE slae, double[] x0);
}
public interface ISLAE
{
    IMatrix Matrix { get; init; }
    double[] RightPart { get; init; }
    double CalcResidual(double[] result);
}
public interface IMatrix
{
    int Size { get; }
    IEnumerable<(int i,double val)> Diag { get; }
    void LMult(double[] x, double[] res);
    void UMult(double[] x, double[] res);
}
public interface IUserMessager
{
    void TellUser(string message);
}
public abstract partial class SlaeSolver : ISlaeSolver
{
    protected IUserMessager? userMessager { get; }
    public SlaeSolver(double eps, int maxIter, IUserMessager? messager = null)
    {
        Eps = eps;
        MaxIter = maxIter;
        userMessager = messager;
    }
    public double Eps { get; init; } = 1e-10;
    public int MaxIter { get; init; } = 1000;
    protected abstract void BeginSolve(ISLAE slae, double[] x0);
    protected abstract double Iteration(ISLAE slae, double[] current);
    public double[] Solve(ISLAE slae, double[] x0)
    {
        double[] result = x0.ToArray();
        BeginSolve(slae, x0);
        for (int i = 0; i < MaxIter; i++)
        {
            var residual = Iteration(slae, result);
            userMessager?.TellUser($"Iteration: {i}, Residual: {residual}");
            if (residual < Eps) break;
        }
        var residual2 = slae.CalcResidual(result);
        if (residual2 > Eps)
            userMessager?.TellUser($"Iteration stopped, but residual {residual2}");
        return result;
    }
}
public abstract class SparseMatrix : IMatrix
{
    public abstract int Size { get; }
    protected abstract IEnumerable<(int i, int j, double v)> LMatrixElements();
    protected abstract IEnumerable<(int i, int j, double v)> UMatrixElements();

    public void LMult(double[] x, double[] res)
    {
        res.AsSpan().Fill(0);
        foreach (var (i, j, v) in LMatrixElements())
            res[i] += v * x[j];
    }

    public void UMult(double[] x, double[] res)
    {
        res.AsSpan().Fill(0);
        foreach (var (i, j, v) in UMatrixElements())
            res[i] += v * x[j];
    }

    public abstract IEnumerable<(int i, double val)> Diag { get; }
    public static double[] operator *(SparseMatrix a, double[] b)
    {
        var resl = new double[b.Length];
        var resu = new double[b.Length];
        a.LMult(b, resl);
        a.UMult(b, resu);
        var res=new double[b.Length];
        foreach(var d in a.Diag) res[d.i] = b[d.i]*d.val;
        for (int i = 0; i < b.Length; i++)
            res[i] += resl[i] + resu[i];
        return res;
    }
}






