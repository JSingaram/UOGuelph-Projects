Hi.

James Singaram
jsingara@uoguelph.ca
0758172

a3remove.sql and a3data.sql can be run using the "\i <directory>" command. The eight functions can be entered into the database the same way using p#.sql. Afterwards they can be run using "select q##();".


a3remove.sql
	Removes existing customer, transaction, and vendor tables.

a3data.sql
	Creates customer, transaction, and vendor tables.
	Populates customer, transaction, and vendor tables.

p1.sql
	Contains function q01(char) that takes customer name as a parameter.
		Searches for input customer name and returns vendor name, date, and amount of all of their transactions.
		No return value.

p2.sql
	Contains function q02(char) that takes vendor name as a parameter.
		Searches for input vendor name and returns customer numbers, customer names, and provinces of all customers who have purchased from that vendor.
		No return value.

p3.sql
	Contains function q03(char, char, char, int) that takes account number, customer name, province, and credit limit as parameters.
		Creates a customer using the parameters, gives them a balance of zero, and adds them to the customer table.
		No return value.

p4.sql
	Contains function q04() that takes no parameters.
		Displays every customer's account number, name, and the amount and vendor name of their most recent transaction. If no transaction has occurred, displays acount number, name, and a "no transaction" message.
		No return value.

p5.sql
	Contains function q05() that takes no parameters.
		Calculates the total amount from transactions for all vendors and adds that value to each vendor's current balance. Displays vendor numbers, vendor names, and the updated balances.
		No return value.

p6.sql
	Contains function q06() that takes no parameters.
		Calculates a 4% fee for each vendor's balance, subtracts it from their balance, then displays each vendor's name, their fee, and their updated balance.
		No return value.

p7.sql
	Contains function q07() that takes no parameters.
		Finds customers whose balance exceed their limit, calculates a 10% fee for for the amount the balance exceeds their limit, adds it to their outstanding balance, then displays those customers' names and updated balances.
		No return value.

p8.sql
	Contains function q08(char, char, real) that takes a new transaction's vendor number, acocunt number, and transaction amount as parameters.
		If the vendor and account numbers are valid, a new transaction is added to the transaction table using the current date and the input values. Customer and vendor balances are updated. Displays the new transaction, the customer, and the vendor.
		No return value.

