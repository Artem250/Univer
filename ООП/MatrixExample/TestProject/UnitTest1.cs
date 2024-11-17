using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace TestProject
{
    [TestClass]
    public class UnitTest
    {
        SquareMatrix testMatrix;

        public UnitTest()
        {
            testMatrix = new SquareMatrix(2);
            testMatrix[0, 0] = 1;
            testMatrix[0, 1] = 2;
            testMatrix[1, 0] = 3;
            testMatrix[1, 1] = 4;
        }

        [TestMethod]
        public void LMultXSize2()
        {
            double[] res = new double[2];
            testMatrix.LMult(new double[] { 1, 2 }, res);
            Assert.AreEqual(2, res.Length);
            Assert.AreEqual(0, res[0], 1e-15);
            Assert.AreEqual(3, res[1], 1e-15);
        }

        [TestMethod]
        public void UMultXSize2()
        {
            double[] res = new double[2];
            testMatrix.UMult(new double[] { 1, 2 }, res);
            Assert.AreEqual(2, res.Length);
            Assert.AreEqual(4, res[0], 1e-15);
            Assert.AreEqual(0, res[1], 1e-15);
        }

        [TestMethod]
        public void MultXSize2()
        {
            var res = testMatrix * new double[] { 1, 2 };
            Assert.AreEqual(2, res.Length);
            Assert.AreEqual(5, res[0], 1e-15);
            Assert.AreEqual(11, res[1], 1e-15);
        }

        [TestMethod]
        public void LMultXWithInvalidSize_ThrowsArgumentException()
        {
            double[] res = new double[1];
            Assert.ThrowsException<ArgumentException>(() => testMatrix.LMult(new double[] { 1 }, res));
        }

        [TestMethod]
        public void UMultXWithInvalidSize_ThrowsArgumentException()
        {
            double[] res = new double[1];
            Assert.ThrowsException<ArgumentException>(() => testMatrix.UMult(new double[] { 1 }, res));
        }

        [TestMethod]
        public void MultWithDifferentSizeMatrix_ThrowsArgumentException()
        {
            SquareMatrix matrix3x3 = new SquareMatrix(3);
            matrix3x3[0, 0] = 1;
            matrix3x3[0, 1] = 2;
            matrix3x3[0, 2] = 3;
            matrix3x3[1, 0] = 4;
            matrix3x3[1, 1] = 5;
            matrix3x3[1, 2] = 6;
            matrix3x3[2, 0] = 7;
            matrix3x3[2, 1] = 8;
            matrix3x3[2, 2] = 9;

            Assert.ThrowsException<ArgumentException>(() => testMatrix * new double[] { 1, 2, 3 });
        }

        [TestMethod]
        public void DiagonalElementsTest()
        {
            var diagonalElements = testMatrix.Diag.ToArray();
            Assert.AreEqual(2, diagonalElements.Length);
            Assert.AreEqual((0, 1.0), diagonalElements[0]);
            Assert.AreEqual((1, 4.0), diagonalElements[1]);
        }

        [TestMethod]
        public void EmptyMatrixTest()
        {
            var emptyMatrix = new SquareMatrix(0);
            double[] res = new double[0];
            Assert.ThrowsException<ArgumentException>(() => emptyMatrix.LMult(new double[0], res));
            Assert.ThrowsException<ArgumentException>(() => emptyMatrix.UMult(new double[0], res));
            Assert.ThrowsException<ArgumentException>(() => emptyMatrix * new double[0]);
        }
    }
}
