using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlaeSolvers
{
    public static class VectorOperators
    {
        public static double Dot(this ReadOnlySpan<double> a, ReadOnlySpan<double> b)
        {
            double s = 0;
            for(int i=0;i<a.Length;i++)s+= a[i] * b[i];
            return s;
        }
        public static double Dot(this double[] a, ReadOnlySpan<double> b)
        {
            double s = 0;
            for (int i = 0; i < a.Length; i++) s += a[i] * b[i];
            return s;
        }
    }
    public abstract class IterationSolver : ISlaeSolver
    {
        public int MaxIter { get; init; } = 1000;
        public double Eps { get; init; } = 1e-10;
        public abstract string MethodName { get; }
        public abstract void Iteration(ISLAE slae, ReadOnlySpan<double> XK0, Span<double> XK1);
        public void Solve(ISLAE slae, Span<double> solution, IPrinter printer)
        {
            var work = new double[slae.RightPart.Length];
            var rpNorm=Math.Sqrt(slae.RightPart.Dot(slae.RightPart));

            printer.Print($"Begin solve {MethodName}");
            for (int iter = 0; iter < MaxIter; iter++)
            {
                Iteration(slae, solution, work);

                var residual=slae.Residual(work);
                var r = Math.Sqrt(residual.Dot(residual));
                r = Math.Sqrt(r) / rpNorm;
                printer.PrintSolverIteration(iter, r, Eps);
                work.CopyTo(solution);
                if (r < Eps) break;
            }
            printer.Print($"\nFinish solve {MethodName}");
        }
    }
    public class JacobiSolver:IterationSolver
    {
        public override string MethodName => "Jacoby";
        public override void Iteration(ISLAE slae, ReadOnlySpan<double> XK0, Span<double> XK1)
        {
            var work = new double[slae.RightPart.Length];
            slae.Matrix.LMult(XK0, XK1);
            XK1.CopyTo(work);
            slae.Matrix.UMult(XK0, XK1);
            for (int i = 0; i < XK1.Length; i++)
                work[i] += XK1[i];
            for (int i = 0; i < XK1.Length; i++)
                work[i]=slae.RightPart[i]-work[i];
            slae.Matrix.DInverseMult(work, XK1);
        }
    }
}
