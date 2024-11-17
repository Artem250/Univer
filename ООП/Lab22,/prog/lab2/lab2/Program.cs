using System;


Console.WriteLine("Адис-абебе");
public interface IPerson
{
    string Name { get; }
    string Patronomic { get; }
    string Lastname { get; }
    DateTime Date { get; }
    int Age { get; }
}

public class Student : IPerson
{
    public string Name { get; }
    public string Patronomic { get; }
    public string Lastname { get; }
    public DateTime Date { get; }
    public int Age { get; }
    public int Course { get; }
    public string Group { get; }
    public float Score { get; }

    public Student(string name, string patronomic, string lastname, DateTime date, int age, int course, string group, float score)
    {
        Name = name;
        Patronomic = patronomic;
        Lastname = lastname;
        Date = date;
        Age = age;
        Course = course;
        Group = group;
        Score = score;
    }
    public static Student FromString(string input)
    {
        var array = input.Split(", ", StringSplitOptions.RemoveEmptyEntries);
        return new Student(array[0], array[1], array[2], DateTime.Parse(array[3]), int.Parse(array[4]), int.Parse(array[5]), array[6], float.Parse(array[7]));
    }
    public override string ToString()
    {
        return $"{Lastname} {Name} {Patronomic}, Возраст: {Age}, " +
            $"Курс: {Course}, Группа: {Group}, Средний балл: {Score}";
    }
}
public enum Position
{ 
    Lecturer,
    Professor,
    Laboratorian,
    Dean,
    Rector
}
public class Teacher : IPerson
{
    public string Name { get; }
    public string Patronomic { get; }
    public string Lastname { get; }
    public DateTime Date { get; }
    public int Age { get; }
    public string Department { get; }
    public int Seniority { get; }
    public Position Job { get; }

    public Teacher(string name, string patronomic, string lastname, DateTime date, int age, string department, int seniority, Position job)
    {
        Name = name;
        Patronomic = patronomic;
        Lastname = lastname;
        Date = date;
        Age = age;
        Department = department;
        Seniority = seniority;
        Job = job;
    }
    public static Teacher FromString(string input)
    {
        var array = input.Split(", ", StringSplitOptions.RemoveEmptyEntries);
        return new Teacher(array[0], array[1], array[2], DateTime.Parse(array[3]), int.Parse(array[4]), array[5], int.Parse(array[6]), 
            (Position)Enum.Parse(typeof(Position), array[7]));
    }
    public override string ToString()
    {
        return $"{Lastname} {Name} {Patronomic}, Возраст: {Age}, " +
            $"Кафедра: {Department}, Стаж: {Seniority}, Должность: {Job}";
    }
}

//Чё с University делать я ваще не понял
public interface IUniversity
{
    IEnumerable<IPerson> Persons { get; }   // отсортировать в соответствии с вариантом 1-й лабы
    IEnumerable<Student> Students { get; }  // отсортировать в соответствии с вариантом 1-й лабы
    IEnumerable<Teacher> Teachers { get; }  // отсортировать в соответствии с вариантом 1-й лабы

    void Add(IPerson person);
    void Remove(IPerson person);

    IEnumerable<IPerson> FindByLastName(string lastName);

    // Для нечетных вариантов. Выдать всех студентов, чей средний балл выше заданного.
    // Отсортировать по среднему баллу
    IEnumerable<Student> FindByAvrPoint(float avrPoint);
}

public class University : IUniversity
{
    public List<IPerson> persons = new List<IPerson>();

    persons.Sort((x, y) => string.Compare(x.Name, y.Name));
    //public IEnumerable<IPerson> Persons => persons.Sort();

    public void Add(IPerson person)
    {
        throw new NotImplementedException();
    }

    public void Remove(IPerson person)
    {
        throw new NotImplementedException();
    }

    public IEnumerable<IPerson> FindByLastName(string lastName)
    {
        throw new NotImplementedException();
    }

    public IEnumerable<Student> FindByAvrPoint(float avrPoint)
    {
        throw new NotImplementedException();
    }

    private IEnumerable<Student> Students { get; }
    private IEnumerable<Teacher> Teachers { get; }
    private IEnumerable<IPerson> Persons { get; }

    IEnumerable<IPerson> IUniversity.Persons => throw new NotImplementedException();

    IEnumerable<Student> IUniversity.Students => throw new NotImplementedException();

    IEnumerable<Teacher> IUniversity.Teachers => throw new NotImplementedException();
    //абоба.
}