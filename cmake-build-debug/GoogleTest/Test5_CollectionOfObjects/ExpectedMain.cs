class Program {
	public static void Main(string[] args) {
		Class1 man1 = new Class1("Jan", null, "Hello");
		Class1 man2 = new Class1("Pawel", null, "Hi");
		Class1 man3 = new Class1("TheSecond", "Pope", "Good day!");
		List<Class1> Class1_objects = new List<Class1>(3);
		Class1_objects.Add(man1);
		Class1_objects.Add(man2);
		Class1_objects.Add(man3);
		Class2 queue = new Class2("Shop", Class1_objects);
		Class3 root = new Class3(queue);
	}
}
