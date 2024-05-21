class Program {
	public static void Main(string[] args) {
		Class1 message = new Class1("Hello", "Mum");
		Class2 call = new Class2("today", "Daughter", "Hello");
		Class3 root = new Class3(message, call);
	}
}
