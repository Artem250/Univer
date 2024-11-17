
// 1. ввод слау
// 2. решение
// 3. вывод результата

public class SquareMatrix : SparseMatrix
{
    private double[,] matrix;
    public SquareMatrix(int size)
    {
        matrix = new double[size, size];
    }
    public double this[int i,int j] { get => matrix[i,j]; set => matrix[i, j] = value; }
    protected override IEnumerable<(int i, int j, double v)> LMatrixElements()
    {
        for (int i = 0; i < Size; i++)
            for (int j = 0; j < i; j++)
                yield return (i,j,matrix[i, j]);
    }
    protected override IEnumerable<(int i, int j, double v)> UMatrixElements() 
    {
        for (int i = 0; i<Size; i++)
            for (int j = 0; j<i; j++)
                yield return (j, i, matrix[j, i]);
    }

public override int Size => matrix.GetLength(0);

    public override IEnumerable<(int i, double val)> Diag
    {
        get
        {
            for (int i = 0; i < Size; i++)
                    yield return (i, matrix[i, i]);
        }
    }
}




