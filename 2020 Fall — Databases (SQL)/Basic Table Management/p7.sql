/*
	Execute Problem 7
*/


create or replace function q07() returns void as $$
	declare
		-- Create cursors
		c1 cursor for 
			select cname, cbalance, crlimit
			from customer
			where (cbalance > crlimit);
		
		-- Create variables
		customer_name char(50);
		fee real;
		customer_balance real;
		customer_limit int;
		updated_balance real;

	--Start executable portion of function
	begin
		open c1;
		loop

			fetch c1 into customer_name, customer_balance, customer_limit;
			exit when not found;

			fee = 0.1 * (customer_balance - customer_limit);

			updated_balance = customer_balance + fee;

			raise notice 'Customer name: %', customer_name;
			raise notice 'Fee: %', fee;
			raise notice 'Updated balance: %', updated_balance;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language plpgsql;