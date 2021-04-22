/*
	Execute Problem 6
*/


create or replace function q06() returns void as $$
	declare
		-- Create cursors
		c1 cursor for 
			select vname, vbalance
			from vendor;
		
		-- Create variables
		vendor_name char(50);
		fee real;
		vendor_balance real;
		updated_balance real;

	--Start executable portion of function
	begin
		open c1;
		loop

			fetch c1 into vendor_name, vendor_balance;
			exit when not found;

			fee = 0.04 * vendor_balance;

			updated_balance = vendor_balance - fee;

			raise notice 'Vendor name: %', vendor_name;
			raise notice 'Fee: %', fee;
			raise notice 'Updated balance: %', updated_balance;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language plpgsql;