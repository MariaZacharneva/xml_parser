class Program {
	public static void Main(string[] args) {
		Class1 message = new Class1(null, "Hello", "Mum");
		Class2 a = new Class2(message);
		Class1 call = new Class1("Daughter", "Hi!", "Dad");
		Class3 b = new Class3(call);
		Class4 root = new Class4(a, b);
	}
}
