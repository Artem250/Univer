using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlaeSolvers
{
    public class DenseMatrix:BaseMatrix
    {
        public required double[,] Matr { get; init; }
        protected override IEnumerable<(int i, int j, double element)> Values
        {
            get
            {
                for (int i = 0; i < Matr.GetLength(0); i++)
                    for (int j = 0; j < Matr.GetLength(1); j++)
                        yield return (i, j, Matr[i, j]);

            }
        }
        public override void LDInverseMult(ReadOnlySpan<double> vector, Span<double> res) => throw new NotImplementedException();
    }
}
