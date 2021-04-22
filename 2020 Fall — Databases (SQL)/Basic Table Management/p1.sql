/*
	Execute Problem 1
*/


create or replace function q01(name char) returns void as $$
	declare
		-- Create variables
		c1 cursor for 
			select vname, t_date, amount 
			from customer natural join transaction natural join vendor 
			where (cname = name);

		vendor_name char(50);
		transaction_date date;
		num real;

	--Start executable portion of function
	begin
		open c1;
		loop


			fetch c1 into vendor_name, transaction_date, num;

			exit when not found;

			raise notice 'Vendor name: %', vendor_name;
			raise notice 'Transaction date: %', transaction_date;
			raise notice 'Amount name: %', num;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language 'plpgsql';