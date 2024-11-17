// See https://aka.ms/new-console-template for more information
Console.WriteLine("Hello, World!");
double[] array = new double[1000];
ReadOnlySpan<double> span = array.AsSpan(100, 200);
