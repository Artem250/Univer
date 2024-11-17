using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlaeSolvers
{
    public abstract class BaseMatrix:IMatrix
    {
        protected abstract IEnumerable<(int i,int j,double element)> Values { get; }
        public virtual void Mult(ReadOnlySpan<double> vector, Span<double> res)
        {
            res.Fill(0);
            foreach (var p in Values)
                res[p.i]+= vector[p.j]*p.element;
        }
        public virtual void LMult(ReadOnlySpan<double> vector, Span<double> res)
        {
            res.Fill(0);
            foreach (var p in Values)
                if(p.i > p.j) 
                    res[p.i] += vector[p.j] * p.element;
        }
        public virtual void UMult(ReadOnlySpan<double> vector, Span<double> res)
        {
            res.Fill(0);
            foreach (var p in Values)
                if (p.i < p.j)
                    res[p.i] += vector[p.j] * p.element;
        }
        public virtual void DMult(ReadOnlySpan<double> vector, Span<double> res)
        {
            res.Fill(0);
            foreach (var p in Values)
                if (p.i == p.j)
                    res[p.i] += vector[p.j] * p.element;
        }
        public virtual void DInverseMult(ReadOnlySpan<double> vector, Span<double> res)
        {
            res.Fill(0);
            foreach (var p in Values)
                if (p.i == p.j)
                    res[p.i] += vector[p.j] / p.element;
        }
        public abstract void LDInverseMult(ReadOnlySpan<double> vector, Span<double> res);

    }
}
