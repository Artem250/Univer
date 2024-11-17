using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlaeSolvers
{
    public interface ISlaeSolver
    {
        void Solve(ISLAE slae, Span<double> solution, IPrinter printer);
    }
    public interface ISLAE
    {
        IMatrix Matrix { get; }
        double[] RightPart { get; }

        double[] Residual(ReadOnlySpan<double> x);
    }
    public interface IPrinter
    {
        void Print(string s);
        void PrintSolverIteration(int iter, double residual, double eps);
    }

    public interface IMatrix
    {
        void Mult(ReadOnlySpan<double> vector,Span<double> res);
        void LMult(ReadOnlySpan<double> vector, Span<double> res);
        void UMult(ReadOnlySpan<double> vector, Span<double> res);
        void DMult(ReadOnlySpan<double> vector, Span<double> res);
        void DInverseMult(ReadOnlySpan<double> vector, Span<double> res);
        void LDInverseMult(ReadOnlySpan<double> vector, Span<double> res);
    }

}
