University university = new University();



while (true)
{
    Console.WriteLine("Выберите действие:");
    Console.WriteLine("1. Добавить студента");
    Console.WriteLine("2. Добавить преподавателя");
    Console.WriteLine("3. Добавить людей в базу из файла");
    Console.WriteLine("4. Удалить человека по фамилии");
    Console.WriteLine("5. Вывести актуальный список людей в консоль");
    Console.WriteLine("6. Сохранить базу в файл");
    Console.WriteLine("7. Найти по фамилии");
    Console.WriteLine("8. Найти студентов по среднему баллу");
    Console.WriteLine("0. Выход");

    var choice = Console.ReadLine();
    switch (choice)
    {
        case "1":
            Console.WriteLine("Введите данные студента: Имя, Отчество, Фамилия, Дата рождения, Курс, Группа, Средний балл");
            university.Add(Student.FromString(Console.ReadLine()));
            break;
        case "2":
            Console.WriteLine("Введите данные преподавателя: Имя, Отчество, Фамилия, Дата рождения, Кафедра, Стаж, Должность");
            university.Add(Teacher.FromString(Console.ReadLine()));
            break;
        case "3":
            var input = File.ReadAllLines("input.txt");
            try
            {
                foreach (var str in input)
                {
                    var spisok = str.Split(": ", StringSplitOptions.RemoveEmptyEntries);
                    university.Add(spisok[0] == "П" ? Teacher.FromString(spisok[1]) : Student.FromString(spisok[1]));
                }
            }
            catch
            {
                throw new Exception("Неверно введены данные. Формат ввода данных:\n" +
                    "Для студентов: 'С: Имя Отчество Фамилия Дата рождения Курс Группа Средний балл'\n" +
                    "Для преподавателей: 'П: Имя Отчество Фамилия Дата рождения Кафедра Стаж Должность')");
            }
            Console.WriteLine("Success");
            break;
        case "4":
            Console.WriteLine("Введите фамилию человека: ");
            university.Remove(Console.ReadLine());
            break;
        case "5":
            foreach (var str in university.Persons)
                Console.WriteLine(str.ToString());
            break;
        case "6":
            List<string> output = new List<string>();
            foreach (var str in university.Persons)
                output.Add(str.ToString());
            File.WriteAllLines("output.txt", output);
            Console.WriteLine("Success");
            break;
        case "7":
            Console.Write("Введите фамилию для поиска: ");
            var foundPersons = university.FindByLastName(Console.ReadLine());
            foreach (var fp in foundPersons)
                Console.WriteLine(fp);
            break;
        case "8":
            Console.Write("Введите средний балл для поиска: ");
            var highScorers = university.FindByAvrPoint(float.Parse(Console.ReadLine()));
            foreach (var s in highScorers)
                Console.WriteLine(s);
            break;
        case "0":
            return;
        default:
            Console.WriteLine("Неверный выбор. Попробуйте снова.");
            break;
    }
    Console.WriteLine();
}
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
    DateTime Today = DateTime.Now;
    public int Age => Today.Year - Date.Year -
        (Today.Month < Date.Month || (Today.Month == Date.Month && Today.Day < Date.Day) ? 1 : 0);
    public int Course { get; }
    public string Group { get; }
    public float Score { get; }


    public Student(string name, string patronomic, string lastname, DateTime date, int course, string group, float score)
    {
        Name = name;
        Patronomic = patronomic;
        Lastname = lastname;
        Date = date;
        Course = course;
        Group = group;
        Score = score;
    }
    public static Student FromString(string input)
    {
        var array = input.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        return new Student(array[0], array[1], array[2], DateTime.Parse(array[3]), int.Parse(array[4]), array[5],
            float.Parse(array[6]));
    }
    public override string ToString()
    {
        return $"{Lastname} {Name} {Patronomic}, Дата рождения: {Date.ToString("dd.MM.yyyy")}, " +
            $"Курс: {Course}, Группа: {Group}, Средний балл: {Score}";
    }
}
public enum Position
{
    Лектор,
    Профессор,
    Лаборант,
    Декан,
    Ректор
}
public class Teacher : IPerson
{
    public string Name { get; }
    public string Patronomic { get; }
    public string Lastname { get; }
    public DateTime Date { get; }
    DateTime Today = DateTime.Now;
    public int Age => Today.Year - Date.Year -
        (Today.Month < Date.Month || (Today.Month == Date.Month && Today.Day < Date.Day) ? 1 : 0);
    public string Department { get; }
    public int Seniority { get; }
    public Position Job { get; }

    public Teacher(string name, string patronomic, string lastname, DateTime date, string department, int seniority, Position job)
    {
        Name = name;
        Patronomic = patronomic;
        Lastname = lastname;
        Date = date;
        Department = department;
        Seniority = seniority;
        Job = job;
    }
    public static Teacher FromString(string input)
    {
        var array = input.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        return new Teacher(array[0], array[1], array[2], DateTime.Parse(array[3]), array[4], int.Parse(array[5]),
            (Position)Enum.Parse(typeof(Position), array[6]));
    }
    public override string ToString()
    {
        return $"{Lastname} {Name} {Patronomic}, Дата рождения: {Date.ToString("dd.MM.yyyy")}, " +
            $"Кафедра: {Department}, Стаж: {Seniority}, Должность: {Job}";
    }
}

public interface IUniversity
{
    IEnumerable<IPerson> Persons { get; }
    IEnumerable<Student> Students { get; }
    IEnumerable<Teacher> Teachers { get; }
    void Add(IPerson person);
    void Remove(string person);
    IEnumerable<IPerson> FindByLastName(string lastName);
    IEnumerable<Student> FindByAvrPoint(float avrPoint);
}

public class University : IUniversity
{
    private List<IPerson> persons = new List<IPerson>();

    public IEnumerable<IPerson> Persons => persons.OrderBy(x => x.Lastname);
    public IEnumerable<Student> Students => persons.OfType<Student>().OrderBy(x => x.Lastname);
    public IEnumerable<Teacher> Teachers => persons.OfType<Teacher>().OrderBy(x => x.Lastname);

    public void Add(IPerson person) => persons.Add(person);
    public void Remove(string person)
    {
        IEnumerable<IPerson> people = FindByLastName(person);
        if (people.Any())
        {
            Console.WriteLine("Выберите человека, которого хотите удалить из базы");
            int num = 1;
            foreach (IPerson person1 in people)
                Console.WriteLine($"{num++}) {person1}");
            int choice = int.Parse(Console.ReadLine());

            if (choice != 0 && choice < num)
            {
                persons.Remove(people.ToArray()[choice - 1]);
                Console.WriteLine("Success");
            }
            else
                Console.WriteLine("Человека с таким номером нет в списке, попробуйте ещё раз");
        }
        else
            Console.WriteLine("Такого человека  нет в базе");
    }


    public IEnumerable<IPerson> FindByLastName(string lastName) => persons.Where(x => x.Lastname.Equals
        (lastName, StringComparison.OrdinalIgnoreCase));
    public IEnumerable<Student> FindByAvrPoint(float avrPoint) => Students.Where
        (x => x.Score > avrPoint).OrderBy(x => x.Score);
}

