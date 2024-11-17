using System;
using System.IO;
using System.Reflection.Metadata;
using System.Security.Cryptography;
using System.Text;
using static System.Runtime.InteropServices.JavaScript.JSType;

public class AESCipher
{
    public static byte[] ComputeSha256Hash(string rawData)
    {
        using (SHA256 sha256Hash = SHA256.Create())
        {
            return sha256Hash.ComputeHash(Encoding.UTF8.GetBytes(rawData));
        }
    }

    public static byte[] EncryptAES(string plainText, byte[] Key, byte[] iv)
    {

        byte[] encrypted;

        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = iv;
            aesAlg.Mode = CipherMode.CBC;
            aesAlg.Padding = PaddingMode.PKCS7;

            ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

            using (MemoryStream msEncrypt = new MemoryStream())
            {
                using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                {
                    using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                    {
                        swEncrypt.Write(plainText);
                    }
                    encrypted = msEncrypt.ToArray();
                }
            }
        }

        return encrypted;
    }

    public static string DecryptAES(byte[] cipherText, byte[] Key, byte[] IV)
    {

        string plaintext = null;

        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = IV;
            aesAlg.Mode = CipherMode.CBC;
            aesAlg.Padding = PaddingMode.PKCS7;

            ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

            using (MemoryStream msDecrypt = new MemoryStream(cipherText))
            {
                using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                {
                    using (StreamReader srDecrypt = new StreamReader(csDecrypt))
                    {
                        plaintext = srDecrypt.ReadToEnd();
                    }
                }
            }
        }

        return plaintext;
    }

    
}


public class Program
{
    public static void Main()
    {
        int action = 0;
        while (action != 3)
        {
        Console.Write("Доступны следующие действия:\n" +
            "1)Зашифровать текст из файла\n" +
            "2)Расшифровать текст из файла\n" +
            "3)Выйти из приложения\n" +
            "Выберите действие:");
        while ((!int.TryParse(Console.ReadLine(), out action)) || action < 1 || action > 3)
            Console.WriteLine("В списке действий нет соответствующего пункта. Введите число заново:");

            switch (action)
            {
                case 1:
                    {
                        Console.Write("Введите имя файла для чтения текста: ");
                        string inputFileName = Console.ReadLine();

                        string original = File.ReadAllText(inputFileName);

                        Console.Write("Введите пароль: ");
                        string password = Console.ReadLine();

                        byte[] key = AESCipher.ComputeSha256Hash(password);

                        byte[] iv;
                        using (Aes aes = Aes.Create())
                        {
                            aes.GenerateIV();
                            iv = aes.IV;
                        }

                        byte[] encrypted = AESCipher.EncryptAES(original, key, iv);

                        string encryptedBase64 = Convert.ToBase64String(encrypted);
                        string ivBase64 = Convert.ToBase64String(iv);

                        string outputEncryptedFileName = "text.txt";
                        Console.WriteLine($"Зашифрованный текст выведен в файл {outputEncryptedFileName}");
                        File.WriteAllText(outputEncryptedFileName, encryptedBase64);

                        string outputIvFileName = "iv.txt";
                        Console.WriteLine($"Зашифрованный текст выведен в файл {outputIvFileName}");
                        File.WriteAllText(outputIvFileName, ivBase64);
                        break;
                    }
                case 2:
                    {
                        Console.Write("Введите имя файла для чтения зашифрованного текста: ");
                        string inputFileName = Console.ReadLine();
                        string encryptedStr = File.ReadAllText(inputFileName);
                        byte[] encrypted = Convert.FromBase64String(encryptedStr);

                        Console.Write("Введите пароль: ");
                        string password = Console.ReadLine();
                        byte[] key = AESCipher.ComputeSha256Hash(password);

                        Console.Write("Введите имя файла содержащего IV: ");
                        string ivFileName = Console.ReadLine();

                        string ivStr = File.ReadAllText(ivFileName);
                        byte[] iv = Convert.FromBase64String(ivStr);

                        string decrypted = AESCipher.DecryptAES(encrypted, key, iv);
                        string outputDecryptedFileName = "encrypted.txt";
                        Console.WriteLine($"Расшифрованный текст выведен в файл {outputDecryptedFileName}");
                        File.WriteAllText(outputDecryptedFileName, decrypted);
                        break;
                    }
                case 3:
                    {
                        return;
                    }
            }
        }




    }
}