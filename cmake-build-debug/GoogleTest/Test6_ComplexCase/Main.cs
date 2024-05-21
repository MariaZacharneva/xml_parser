class Program {
	public static void Main(string[] args) {
		Class1 mother = new Class1("1970-02-02", "Ann", "Brown", null);
		List<Class1> Class1_objects = new List<Class1>(1);
		Class1_objects.Add(mother);
		Class1 newman = new Class1("1990-01-01", "John", "Newman", Class1_objects);
		Class1 mother = new Class1("1970-02-02", "mARY", "Novak", null);
		Class1 father = new Class1("1970-02-02", "Henry", "Novak", null);
		List<Class1> Class1_objects = new List<Class1>(2);
		Class1_objects.Add(father);
		Class1_objects.Add(mother);
		Class1 novak = new Class1("2005-11-16", "Matt", "Novak", Class1_objects);
		Class2 cat1 = new Class2("Jerry", "Tom");
		Class2 cat2 = new Class2("Odie", "Garfield");
		List<Class2> Class2_objects = new List<Class2>(2);
		Class2_objects.Add(cat1);
		Class2_objects.Add(cat2);
		Class3 cats = new Class3(Class2_objects);
		List<Class1> Class1_objects = new List<Class1>(2);
		Class1_objects.Add(newman);
		Class1_objects.Add(novak);
		Class4 root = new Class4(Class1_objects, cats);
	}
}
