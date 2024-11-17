
public class RowSparseMatrix : SparseMatrix
{
    int[] ia;
    int[] ja;
    double[] diag;
    double[] a;
    public RowSparseMatrix(int size)
    {
        ia = new int[size + 1];
        ja = new int[0];
        diag = new double[size];
        a = new double[0];
    }
    public RowSparseMatrix(int[] ia, int[] ja,double[] diag, double[]a )
    {
        this.ia = ia;
        this.ja = ja;
        this.diag = diag;
        this.a = a;
    }

    public override int Size => diag.Length;

    protected override IEnumerable<(int i, int j, double v)> LMatrixElements()
    {
        for (int i = 0; i < Size; i++)
            for (int jind = ia[i]; jind < ia[i + 1]; jind++)
            {
                var j = ja[jind];
                if (i > j) yield return (i, j, a[jind]);
            }
    }
    protected override IEnumerable<(int i, int j, double v)> UMatrixElements()
    {
        for (int i = 0; i < Size; i++)
            for (int jind = ia[i]; jind < ia[i + 1]; jind++)
            {
                var j = ja[jind];
                if (i < j) yield return (i, j, a[jind]);
            }
    }

    public override IEnumerable<(int i, double val)> Diag
    {
        get
        {
            for (int i = 0; i < Size; i++)
                yield return (i, diag[i]);
        }
    }
}




