using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlaeSolvers
{
    public class ConsolePrinter:IPrinter
    {
        public int Skip { get; init; } = 1;
        public void Print(string s) => Console.WriteLine(s);
        public void PrintSolverIteration(int iter, double residual, double eps)
        {
            if(iter%Skip == 0) 
                Console.Write($"\r Iteration:{iter}, residual: {residual} eps:{eps}");
        }
    }
}
