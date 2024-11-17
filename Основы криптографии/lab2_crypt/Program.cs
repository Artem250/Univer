using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text.RegularExpressions;
using static System.Net.Mime.MediaTypeNames;
using static System.Runtime.InteropServices.JavaScript.JSType;

public class Text
{
    private static readonly string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    private List<string> lines;
    private List<string> encryptedLines;
    private string key;
    public Text()
    {
        lines = new List<string>();
        encryptedLines = new List<string>();
        key = FisherYatesShuffle();
    }
    public void AddLine(string line)
    {
        lines.Add(line);
    }

    public static string FisherYatesShuffle() //Алгоритм Шифрования Фишера-Йетса
    {
        Random rand = new Random();
        char[] shuffledAlphabet = alphabet.ToCharArray();

        for (int i = shuffledAlphabet.Length - 1; i > 0; i--)
        {
            int j = rand.Next(i + 1);
            (shuffledAlphabet[i], shuffledAlphabet[j]) = (shuffledAlphabet[j], shuffledAlphabet[i]);
        }
        return new string(shuffledAlphabet);
    }

    public void TextEncrypt()
    {
        foreach (var line in lines)
        {
            string encryptedLine = StringEncrypt(key, line);
            encryptedLines.Add(encryptedLine);
        }
    }

    private static string StringEncrypt(string key, string text) // Метод для шифрования текста методом простой замены
    {
        string moddedText = Regex.Replace(text.ToLower(), @"[^а-яё]", "");

        var substitutionDictionary = new Dictionary<char, char>(); //Словарь для замены

        for (int i = 0; i < alphabet.Length; i++)
            substitutionDictionary[alphabet[i]] = key[i];

        char[] encryptedText = moddedText.Select(c => substitutionDictionary[c]).ToArray();

        return new string(encryptedText);
    }

    public void ReadText(string filename)
    {
        var lines = File.ReadAllLines(filename);
        foreach (var line in lines)
            AddLine(line);
    }
    public void PrintUnEncryptConsole()
    {
        foreach (var line in lines)
            Console.WriteLine(line);
    }

    public void PrintKeyInFile(string filename)
    {
        using (StreamWriter w = new StreamWriter(filename))
        {
            w.WriteLine(key);
        }
    }
    public void PrintAssayFile(string filename)
    {
        string resString = System.String.Join("", lines);
        using (StreamWriter w = new StreamWriter(filename))
        {
            w.WriteLine(resString);
        }
    }

    public void PrintEncryptFile(string filename)
    {
        string resString = System.String.Join("", encryptedLines);
        using (StreamWriter w = new StreamWriter(filename))
        {
            w.WriteLine(resString);
        }
    }
}
class Program()
{
    static void Main()
    {
        Text txt = new Text();
        txt.ReadText("assay.txt");
        Console.WriteLine("Эссе считано");
        txt.PrintUnEncryptConsole();
        txt.PrintAssayFile("assayStrs");
        txt.PrintKeyInFile("key.txt");
        txt.TextEncrypt();
        txt.PrintEncryptFile("encryptedAssay.txt");
    }
}