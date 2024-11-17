using SlaeSolvers;

namespace TestProject
{
    [TestClass]
    public sealed class Test1
    {
        [TestMethod]
        public void TestMethod1()
        {
            DenseMatrix Matr = new DenseMatrix() { Matr = new double[2, 2] };
            Matr.Matr[0, 0] = 2;
            Matr.Matr[0, 1] = 0;
            Matr.Matr[1, 0] = 0;
            Matr.Matr[1, 1] = 2;
            double[] res = new double[2];
            Matr.Mult([1, 2], res);
            Assert.AreEqual(2, res.Length);
            Assert.AreEqual(2, res[0],1e-10);
            Assert.AreEqual(4, res[1], 1e-10);
        }
        [TestMethod]
        public void TestMethod2()
        {
            DenseMatrix Matr = new DenseMatrix() { Matr = new double[2, 2] };
            Matr.Matr[0, 0] = 3;
            Matr.Matr[0, 1] = 0;
            Matr.Matr[1, 0] = 0;
            Matr.Matr[1, 1] = 2;
            double[] res = new double[2];
            Matr.Mult([1, 2], res);
            Assert.AreEqual(2, res.Length);
            Assert.AreEqual(2, res[0], 1e-10);
            Assert.AreEqual(4, res[1], 1e-10);
        }
    }
}
