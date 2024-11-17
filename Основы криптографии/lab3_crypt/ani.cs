// using System;
using System.Security.Cryptography;
using System.Text;
// using System.IO;

class RSASignatureExample
{
    public static byte[] SignMessage(string message, RSA privateKey)
    {
        // Хешируем сообщение с использованием SHA256
        byte[] messageBytes = Encoding.UTF8.GetBytes(message);
        using (SHA256 sha256 = SHA256.Create())
        {
            byte[] hash = sha256.ComputeHash(messageBytes);

            // Подписываем хеш с использованием PKCS #1 v1.5
            return privateKey.SignHash(hash, HashAlgorithmName.SHA256, RSASignaturePadding.Pkcs1);
        }
    }

    public static RSA LoadPrivateKey(string privateKeyPath)
    {
        // Загружаем приватный ключ из файла
        string privateKeyText = File.ReadAllText(privateKeyPath);
        RSA privateKey = RSA.Create();
        privateKey.ImportFromPem(privateKeyText.ToCharArray());
        return privateKey;
    }

    static void Main()
    {
        string message = "В университетах преподается множество общеобразовательных дисциплин. " +
            "И хоть студенту обычно и кажется, что это практически бесполезные ему предметы, " +
            "на деле многие из них могут быть применены им в будущем на практике." +
            " Поэтому студентам важно аккуратно расставлять приоритеты при обучении как основным, " +
            "так и второстепенным дисциплинам.";
        // Загружаем приватный ключ из файла 
        RSA privateKey = LoadPrivateKey("client.key");

        // Подписываем сообщение
        byte[] signature = SignMessage(message, privateKey);

        // Выводим результат в Base64
        Console.WriteLine("Message Signature (Base64): " + Convert.ToBase64String(signature));
    }
}



