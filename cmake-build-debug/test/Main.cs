class Program {
	public static void Main(string[] args) {
		Class1 newmanmother = new Class1("1970-02-02", "Ann", "Brown");
		Class2 mother = new Class2(newmanmother);
		Class1 newmanfather = new Class1("1967-12-24", "Joseph", "Newman");
		Class3 father = new Class3(newmanfather);
		Class4 newman = new Class4("1990-01-01", "John", "Newman", mother, father);
		Class1 novak = new Class1("2005-11-16", "Matt", "Novak");
		Class5 tom = new Class5("Jerry", "Tom");
		Class5 garfield = new Class5("Odie", "Garfield");
		Class6 cats = new Class6(garfield);
		Class7 root = new Class7(novak, newman, cats);
	}
}
