/*
	Execute Problem 2
*/


create or replace function q02(vendor_name char) returns void as $$
	declare
		-- Create variables
		c1 cursor for 
			select distinct account, cname, province 
			from customer natural join transaction natural join vendor 
			where (vname = vendor_name);

		account_no char(50);
		customer_name char(50);
		customer_province char(50);

	--Start executable portion of function
	begin
		open c1;
		loop


			fetch c1 into account_no, customer_name, customer_province;

			exit when not found;

			raise notice 'Customer number: %', account_no;
			raise notice 'Customer name: %', customer_name;
			raise notice 'Province: %', customer_province;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language 'plpgsql';