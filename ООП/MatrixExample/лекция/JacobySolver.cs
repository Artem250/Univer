
// 1. ввод слау
// 2. решение
// 3. вывод результата

public class JacobySolver : SlaeSolver
{
    double[]? vector1;
    double[]? vector2;

    public JacobySolver(double eps, int maxIter, IUserMessager? messager = null) : base(eps, maxIter, messager)
    {
    }
    protected override void BeginSolve(ISLAE slae, double[] x0)
    {
        vector1 = new double[slae.Matrix.Size];
        vector2 = new double[slae.Matrix.Size];
    }
    protected override double Iteration(ISLAE slae, double[] current)
    {
        slae.Matrix.UMult(current, vector1!);
        slae.Matrix.LMult(current, vector2!);
        var residual = 0.0;
        foreach (var d in slae.Matrix.Diag)
        {
            var i = d.i;
            var newvalue =
                (slae.RightPart[i] - (vector1![i] + vector2![i])) / d.val;
            var r = newvalue - current[i];
            residual += r * r;
            current[i] = newvalue;
        }
        return residual;
    }
}






