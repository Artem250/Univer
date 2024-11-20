//using System;
//using System.Numerics;
//using System.Security.Cryptography;
//using System.Text;

//class Program
//{
//    static void Main()
//    {
//        Console.WriteLine("Введите число p: ");
//        string? pInput = Console.ReadLine();
//        Console.Write($"p = 0x{pInput}\n");

//        BigInteger p = BigInteger.Parse('0' + pInput, System.Globalization.NumberStyles.HexNumber);
//        Console.WriteLine("Введите число g: ");

//        string? gInput = Console.ReadLine();
//        Console.Write($"g = {gInput}\n");
//        BigInteger g = BigInteger.Parse('0' + gInput);

//        //BigInteger a = GenerateRandomBigInteger(p);
//        string aInput = "2B90A7655DA6497E3D233A14D5FC7C34A0FB23DF13E326B3632D809E010078C4E62C71469A2ABD038644E00F87EE60A12AF84B23069C862FF8E4CE9479C5FC63D227A5AE10383D05825DB613AA15D824A570F88CF30E7A632B0A1ED7CADC1632E6D2EB114DF9D53383933148E871A498C47EEF9FEDF567853BFABF709136A0F7B65869717F5BE1A0643318355B1F818DA25F10955D0C5CC695D4B7FD7FC9861DF0BEF7E72644F2453AF06152AF020B82B44077FC4BC9B24FFC1D73D78BEB384F50988C439662B3AE186168B3BFB1F8BCD0DE35268FB88484219C272BE4DA99347CC4604E08A216315DF26DE157EAA56370A619493D73F1F7F0D7775BAAB855CE";
//        BigInteger a = BigInteger.Parse('0' + aInput, System.Globalization.NumberStyles.HexNumber);

//        Console.WriteLine($"a = 0x{a.ToString("X")}");

//        BigInteger A = BigInteger.ModPow(g, a, p);
//        Console.WriteLine($"A = 0x{A.ToString("X")}");

//        Console.Write("B = 0x");
//        string? bInput = Console.ReadLine();
//        BigInteger B = BigInteger.Parse('0' + bInput, System.Globalization.NumberStyles.HexNumber);

//        BigInteger S = BigInteger.ModPow(B, a, p);
//        Console.WriteLine($"S = 0x{S.ToString("X")}");
//    }


//    static BigInteger GenerateRandomBigInteger(BigInteger p)
//    {
//        Random random = new Random();
//        byte[] randomBytes = new byte[p.GetByteCount()];

//        random.NextBytes(randomBytes);

//        randomBytes[randomBytes.Length - 1] = 0;

//        BigInteger result = new BigInteger(randomBytes) % (p - 3) + 2; //интервал 1 < a < p-1
//        return result;
//    }
//}




using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

class Program
{
    static void Main()
    {
        string privateKey = "client.key";

        string textToSign = "В университетах преподается множество общеобразовательных дисциплин. И хоть студенту обычно и кажется, что это практически бесполезные ему предметы, на деле многие из них могут быть применены им в будущем на практике. Поэтому студентам важно аккуратно расставлять приоритеты при обучении как основ-ным, так и второстепенным дисциплинам.";

        byte[] signature = SignData(textToSign, privateKey);

        Console.WriteLine("Подпись (Base64): " + Convert.ToBase64String(signature));
    }

    static byte[] SignData(string data, string privateKey)
    {
        RSA rsa = RSA.Create();
        rsa.ImportFromEncryptedPem(File.ReadAllText(privateKey), "12345");

        using (SHA256 sha256 = SHA256.Create())
        {
            byte[] hash = sha256.ComputeHash(Encoding.UTF8.GetBytes(data));

            byte[] signature = rsa.SignHash(hash, HashAlgorithmName.SHA256, RSASignaturePadding.Pkcs1);

            return signature;
        }
    }
}



